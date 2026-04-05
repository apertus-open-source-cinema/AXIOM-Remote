"use strict";

import * as THREE from "three/webgpu";
import { GLTFLoader } from "three/examples/jsm/loaders/GLTFLoader.js";
import eventBus from "../eventBus.js";
import { loadFirmware } from "../Firmware.js";
import { Pane } from "tweakpane";

import {
    Fn,
    uv,
    texture,
    uniform,
    vec2,
    vec3,
    fract,
    smoothstep,
    mix,
    float,
    fwidth,
    abs,
    max,
    dot
} from "three/tsl";

// Helper for tracking loading progress
const manager = new THREE.LoadingManager();
manager.onStart = (url, loaded, total) => console.log(`Started loading: ${url}. (${loaded}/${total})`);
manager.onProgress = (url, loaded, total) => console.log(`Loading: ${url}. (${loaded}/${total})`);
manager.onError = (url) => console.error(`Error loading: ${url}`);

export class MainScene {
    // Scene & Viewport
    scene = new THREE.Scene();
    lightTargetPosition = new THREE.Vector3(0.1, 0, -0.1);
    gltfLoader = new GLTFLoader(manager);

    // Model & Animation state
    currentModel = null;
    currentAnimations = null;
    animationMixer = null;
    runningAnimations = 0;
    glowingObjects = [];

    // Media & Textures
    texture = null;
    lcd_pixel_texture = null;

    // Interaction state
    planeZ = new THREE.Plane(new THREE.Vector3(0, 1, 0), 0);
    intersectPoint = new THREE.Vector3();
    m = null;
    firmwareBinder = null;
    tick = (2 * Math.PI) / 12; // 30 degrees per detent
    oldAngle = 0.0;
    knobValue = 0;
    lastMouseAngle = null;
    accumulatedAngle = 0;

    ButtonID = {};
    ButtonState = {};
    button_definitions = null;
    buttonStatePressed = null;
    buttonStateReleased = null;

    // UI & Visuals
    debugOverlayVisible = false;
    lcd = null;
    knob = null;
    knobCenter = new THREE.Vector2();

    raycaster = new THREE.Raycaster();
    mouseDownPosition = new THREE.Vector2();
    buttonPressDistance = 0.001;
    buttonPress = false;
    knobDrag = false;

    lcdUniforms = {
        uBleed: uniform(0.05),
        uPastel: uniform(0.05),
        uGridStrength: uniform(0.08),
        uFallbackBrightness: uniform(0.7),
        uLcdBrightness: uniform(1.75),
        uLcdContrast: uniform(1.55),
        uSaturation: uniform(1.0),
        uGamma: uniform(1.2),
        uResolution: uniform(new THREE.Vector2(320, 240)),
        uTexture: texture(null),
        uLcdMask: texture(null)
    };

    applyLcdSubpixelTSL = Fn(({ uvNode, texNode, params }) => {

        const res = params.uResolution;

        const pixelCoord = uvNode.mul(res);
        const subUv = fract(pixelCoord);

        const texel = vec2(1.0).div(res);
        const offset = texel.y.mul(0.333);

        // Subpixel sampling
        const source = vec3(
            texture(texNode, uvNode.add(vec2(0.0, offset))).r,
            texture(texNode, uvNode).g,
            texture(texNode, uvNode.sub(vec2(0.0, offset))).b
        );

        const mask = texture(params.uLcdMask, subUv).rgb;

        let color = source.mul(mask);

        // --- distance-based fade ---
        const deriv = fwidth(pixelCoord);
        const pixelSize = max(deriv.x, deriv.y);
        const fade = smoothstep(0.3, 0.10, pixelSize);

        const fallback = texture(texNode, uvNode).rgb;

        return mix(fallback, color, fade);
    });

    constructor() {
        manager.onLoad = () => {
            console.log("Loading complete!");
            eventBus.emit("scene_loaded");
        };

        // Load firmware using the default paths defined in Firmware.js
        loadFirmware().then((Module) => {
            this.m = Module;
            console.log("FW: ", this.m);

            this.ButtonID = this.m.ButtonID;
            this.ButtonState = this.m.ButtonState;

            this.SetupDefinitions();

            this.firmwareBinder = new this.m.FirmwareBinder();

            const width = 320;
            const height = 240;

            // Initialize texture for RGBA8888 framebuffer data
            const fbData = new Uint8Array(width * height * 4);
            this.texture = new THREE.DataTexture(
                fbData,
                width,
                height,
                THREE.RGBAFormat,
                THREE.UnsignedByteType
            );

            this.texture.colorSpace = THREE.SRGBColorSpace;
            this.texture.generateMipmaps = false;
            this.lcd_pixel_texture.anisotropy = 1;
            this.texture.magFilter = THREE.NearestFilter;
            this.texture.minFilter = THREE.NearestFilter;
            this.texture.wrapS = THREE.ClampToEdgeWrapping;
            this.texture.wrapT = THREE.ClampToEdgeWrapping;

            this.lcdUniforms.uTexture.value = this.texture;

            // If the model is already loaded, attach the texture now
            if (this.lcd) {
                this.lcd.map = this.texture;
            }
        });

        this.SetupLight();
        this.LoadModel("axiom_remote02.glb");

        this.lcd_pixel_texture = new THREE.TextureLoader().load("data/textures/lcd_pixel.png");

        this.lcd_pixel_texture.wrapS = THREE.RepeatWrapping;
        this.lcd_pixel_texture.wrapT = THREE.RepeatWrapping;
        this.lcd_pixel_texture.magFilter = THREE.NearestFilter;
        this.lcd_pixel_texture.minFilter = THREE.NearestFilter;
        this.lcd_pixel_texture.anisotropy = 1;
        this.lcd_pixel_texture.colorSpace = THREE.SRGBColorSpace;
        this.lcd_pixel_texture.generateMipmaps = false;

        this.lcdUniforms.uLcdMask.value = this.lcd_pixel_texture;

        this.SetupInputListeners();
        this.SetupTweakpane();
    }

    SetupTweakpane() {
        const pane = new Pane();
        const folder = pane.addFolder({ title: 'LCD Shader' });

        folder.addBinding(this.lcdUniforms.uBleed, 'value', { min: 0, max: 0.5, label: 'Bleed' });
        folder.addBinding(this.lcdUniforms.uPastel, 'value', { min: 0, max: 1.0, label: 'Pastel' });
        folder.addBinding(this.lcdUniforms.uGridStrength, 'value', { min: 0, max: 0.2, label: 'Grid' });
        folder.addBinding(this.lcdUniforms.uFallbackBrightness, 'value', { min: 0, max: 2.0, label: 'Fallback Bright' });
        folder.addBinding(this.lcdUniforms.uLcdBrightness, 'value', { min: 0, max: 3.0, label: 'LCD Bright' });
        folder.addBinding(this.lcdUniforms.uLcdContrast, 'value', { min: 0.5, max: 2.0, label: 'LCD Contrast' });
        folder.addBinding(this.lcdUniforms.uSaturation, 'value', { min: 0, max: 2.0, label: 'Saturation' });
        folder.addBinding(this.lcdUniforms.uGamma, 'value', { min: 0.5, max: 2.5, label: 'Gamma' });

        const debugFolder = pane.addFolder({ title: 'Debug' });
        debugFolder.addBinding(this, 'debugOverlayVisible', { label: 'Debug UI' }).on('change', (ev) => {
            if (this.firmwareBinder) {
                this.firmwareBinder.SetDebugMarkersVisible(ev.value);
            }
        });

        pane.on('change', () => {
            eventBus.emit("scene_update_required");
        });
    }

    SetupInputListeners() {
        eventBus.on("input_interaction", (data) => this.handleButton(data.name, data.state));
        eventBus.on("knob_move", (data) => this.handleKnobRotation(data.mouse, data.camera));
        eventBus.on("knob_drag_end", () => { this.lastMouseAngle = null; });
    }

    get sceneNode() {
        return this.scene;
    }

    SetupLight() {
        const ambient = new THREE.AmbientLight(0x444444);
        // this.scene.add(ambient); // Optional: if you want more base light

        // Spotlight 1: Cool accent
        const light1 = new THREE.SpotLight(0xaaaaff, 1.7, 0.3, THREE.MathUtils.degToRad(25), 0.9);
        light1.position.set(0.0, 0.2, 0.15);
        light1.castShadow = true;
        light1.shadow.mapSize.set(2048, 2048);
        light1.shadow.camera.near = 0.1;
        light1.shadow.camera.far = 0.3;
        light1.shadow.radius = 8;
        light1.shadow.bias = -0.01;
        this.scene.add(light1);

        // Spotlight 2: Warm accent
        const light2 = new THREE.SpotLight(0xffbb66, 1.3, 0.4, THREE.MathUtils.degToRad(15), 0.8);
        light2.position.set(-0.2, 0.25, -0.1);
        light2.castShadow = true;
        light2.shadow.mapSize.set(2048, 2048);
        light2.shadow.camera.near = 0.1;
        light2.shadow.camera.far = 0.3;
        light2.shadow.radius = 8;
        light2.shadow.bias = -0.001;
        this.scene.add(light2);
    }

    LoadGLTF(filePath, callback) {
        this.gltfLoader.load(
            filePath,
            callback,
            (xhr) => {
                console.log(`${filePath}: ${(xhr.loaded / xhr.total * 100).toFixed(2)}% loaded`);
            },
            (error) => console.error(error)
        );
    }

    update_lcd() {
        if (!this.texture || !this.firmwareBinder) return;

        const u8 = this.firmwareBinder.GetFB();
        if (!u8) return;

        // Initialize once
        if (!this.texture.image) {
            this.texture.image = { data: u8, width: 320, height: 240 };
        } else {
            this.texture.image.data = u8;
        }

        this.texture.needsUpdate = true;
    }

    SetupDefinitions() {
        // Auto-generate button definitions for buttons 1-12
        this.button_definitions = {};
        for (let i = 1; i <= 12; i++) {
            this.button_definitions[`Button_${i}_DOWN`] = this.m.ButtonID[`BUTTON_${i}`];
            this.button_definitions[`Button_${i}_UP`] = this.m.ButtonID[`BUTTON_${i}`];
        }

        // Special case for the knob
        this.button_definitions.Knob_DOWN = this.m.ButtonID.KNOB;
        this.button_definitions.Knob_UP = this.m.ButtonID.KNOB;
        this.button_definitions.Knob_press_DOWN = this.m.ButtonID.KNOB;
        this.button_definitions.Knob_press_UP = this.m.ButtonID.KNOB;
        this.button_definitions.BUTTON_NONE = this.m.ButtonID.BUTTON_NONE;

        this.buttonStatePressed = this.m.ButtonState.Pressed;
        this.buttonStateReleased = this.m.ButtonState.Released;
    }

    LoadModel(modelFilename) {
        let filePath = "data/models/" + modelFilename;

        this.LoadGLTF(filePath, (gltf) => {
            const gltfScene = gltf.scene;
            this.currentModel = gltfScene;
            this.currentAnimations = gltf.animations;
            this.animationMixer = new THREE.AnimationMixer(this.currentModel);
            this.animationMixer.addEventListener("finished", () => {
                this.runningAnimations--;
                if (this.runningAnimations === 0) {
                    eventBus.emit("scene_animation_finished");
                }
            });

            gltfScene.traverse((child) => {
                if (child.name === "Knob") {
                    this.knob = child;
                    this.knobCenter.set(this.knob.position.x, this.knob.position.z);
                }

                if (child.name.startsWith("Button_") || child.name.startsWith("LED")) {
                    this.glowingObjects.push(child);
                }

                if (child.isMesh) {
                    child.castShadow = true;
                    child.receiveShadow = true;

                    if (child.material && child.material.name === "LCD") {
                        this.lcd = child;
                        const mat = child.material;

                        // Ensure the LCD is opaque
                        mat.transparent = false;
                        mat.alphaTest = 0;
                        mat.depthWrite = true;

                        mat.map = this.texture;
                        mat.emissiveMap = this.texture;
                        mat.roughness = 0.05;
                        mat.metalness = 0.7;
                        mat.emissive = new THREE.Color(0xffffff);
                        mat.emissiveIntensity = 0.4;

                        const lcdNode = this.applyLcdSubpixelTSL({
                            uvNode: uv(),
                            texNode: this.lcdUniforms.uTexture,
                            params: this.lcdUniforms
                        });

                        const nodeMat = new THREE.MeshStandardNodeMaterial({
                            name: "LCD_Modern",
                            roughness: 0.1,
                            metalness: 0.1
                        });

                        nodeMat.colorNode = lcdNode;
                        nodeMat.emissiveNode = lcdNode;
                        nodeMat.emissiveIntensity = 1.5;

                        child.material = nodeMat;
                    }
                }
            });

            console.log("Model loaded successfully");
            this.scene.add(gltfScene);
        });
    }

    handleButton(name, state) {
        if (!this.firmwareBinder || !this.button_definitions) return;

        const buttonKey = `${name}_${state}`;
        const buttonID = this.button_definitions[buttonKey];

        if (buttonID !== undefined) {
            const fwState = state === 'DOWN' ? this.buttonStatePressed : this.buttonStateReleased;
            this.firmwareBinder.Update(buttonID, 0, fwState);
        }
    }

    handleKnobRotation(mouse, camera) {
        if (!this.knob || !this.firmwareBinder) return;

        this.raycaster.setFromCamera(mouse, camera);
        this.raycaster.ray.intersectPlane(this.planeZ, this.intersectPoint);

        const newAngle = Math.atan2(
            this.knobCenter.y - this.intersectPoint.z,
            this.intersectPoint.x - this.knobCenter.x
        ) - Math.PI / 2;

        if (this.lastMouseAngle === null) {
            this.lastMouseAngle = newAngle;
        }

        let deltaMouseAngle = newAngle - this.lastMouseAngle;

        // Normalize delta to [-PI, PI] to prevent huge jumps across quadrants
        if (deltaMouseAngle > Math.PI) deltaMouseAngle -= 2 * Math.PI;
        if (deltaMouseAngle < -Math.PI) deltaMouseAngle += 2 * Math.PI;

        this.lastMouseAngle = newAngle;
        this.accumulatedAngle += deltaMouseAngle;

        while (this.accumulatedAngle > this.tick) {
            this.knobValue--;
            this.accumulatedAngle -= this.tick;
            this.oldAngle -= this.tick;
        }
        while (this.accumulatedAngle < -this.tick) {
            this.knobValue++;
            this.accumulatedAngle += this.tick;
            this.oldAngle += this.tick;
        }

        this.knob.rotation.y = -this.oldAngle;

        if (this.knobValue !== 0) {
            this.firmwareBinder.Update(this.m.ButtonID.KNOB, this.knobValue, this.buttonStateReleased);
            this.knobValue = 0;
            eventBus.emit("scene_update_required");
        }
    }
}
