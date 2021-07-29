"use strict";

import * as THREE from "three";
import { OrbitControls } from "three/examples/jsm/controls/OrbitControls";
import { MainScene } from "./scenes/MainScene.js";
import { HDRCubeTextureLoader } from "three/examples/jsm/loaders/HDRCubeTextureLoader.js";
import PubSub from "pubsub-js";

import {
  SelectiveBloomEffect,
  SSAOEffect,
  EffectComposer,
  EffectPass,
  RenderPass,
  BlendFunction,
  KernelSize,
  BloomEffect,
  SMAAEffect,
} from "postprocessing";

export class App {
  renderContainer = undefined;
  uiContainer = undefined;
  scene = undefined;
  camera = undefined;
  controls = undefined;
  renderer = undefined;

  isRenderingActive = false;

  composer = undefined;

  clock = new THREE.Clock();

  cameraTargetPosition = new THREE.Vector3(0, 0.0, 0);

  selectiveBloomEffect = undefined;

  Init() {
    this.renderContainer = document.getElementById("render_canvas");
    this.uiContainer = document.getElementById("ui");

    this.SetupScene();

    this.SetupGeneral();
    this.SetupRenderer();

    this.SetupHDR();

    this.controls = new OrbitControls(this.camera, this.renderer.domElement);
    this.controls.target.copy(this.cameraTargetPosition);
    this.controls.mouseButtons = {
      MIDDLE: THREE.MOUSE.PAN,
      RIGHT: THREE.MOUSE.ROTATE,
    };
    this.controls.update();

    this.controls.addEventListener("change", () => {
      this.RequestFrame();
    });

    this.SetupPostProcessing();
  }

  SetupGeneral() {
    var aspectRatio =
      this.renderContainer.clientWidth / this.renderContainer.clientHeight;
    console.log("Aspect ratio: " + aspectRatio);

    this.camera = new THREE.PerspectiveCamera(75, aspectRatio, 0.01, 1000);
    this.camera.position.set(-0.3, 0.3, 0.5);
    this.camera.lookAt(this.cameraTargetPosition);
    this.camera.setFocalLength(85);
    this.camera.updateProjectionMatrix();
  }

  SetupRenderer() {
    this.renderer = new THREE.WebGLRenderer({
      //antialias: true,
      canvas: this.renderContainer,
      antialias: false,
      //stencil: false,
      //depth: false,
    });
    this.renderer.setSize(
      this.renderContainer.clientWidth,
      this.renderContainer.clientHeight
    );
    this.renderer.setPixelRatio(window.devicePixelRatio);
    this.renderer.setClearColor(0x223344);

    this.renderer.physicallyCorrectLights = true;
    this.renderer.toneMapping = THREE.LinearToneMapping;
    this.renderer.toneMappingExposure = 1.5;
    this.renderer.gammaFactor = 2.2;
    this.renderer.outputEncoding = THREE.sRGBEncoding;

    this.renderer.shadowMap.enabled = true;
    this.renderer.shadowMap.type = THREE.PCFSoftShadowMap;
  }

  SetupPostProcessing() {
    this.composer = new EffectComposer(this.renderer, {
      frameBufferType: THREE.HalfFloatType,
    });

    this.composer.addPass(new RenderPass(this.scene.scene, this.camera));
    this.selectiveBloomEffect = new SelectiveBloomEffect(
      this.scene.scene,
      this.camera,
      {
        blendFunction: BlendFunction.ADD,
        kernelSize: KernelSize.MEDIUM,
        luminanceThreshold: 0.5,
        luminanceSmoothing: 0.1,
        intensity: 0.5,
      }
    );

    this.selectiveBloomEffect.luminancePass.enabled = false;
    this.selectiveBloomEffect.ignoreBackground = true;

    this.composer.addPass(
      new EffectPass(this.camera, this.selectiveBloomEffect)
    );

    this.composer.multisampling = 4;
  }

  mouse = new THREE.Vector2();

  onMouseMove(event) {
    // calculate mouse position in normalized device coordinates
    // (-1 to +1) for both components
    this.mouse.x = (event.clientX / this.renderContainer.clientWidth) * 2 - 1;
    this.mouse.y = -(event.clientY / this.renderContainer.clientHeight) * 2 + 1;
  }

  SetupScene() {
    this.scene = new MainScene();
    var token = PubSub.subscribe("scene_loaded", () => {
      const selection = this.selectiveBloomEffect.selection;
      this.scene.glowingObjects.forEach((element) => selection.add(element));

      window.addEventListener("pointerdown", (e) => {
        this.scene.processMouseDown(e, this.mouse, this.camera);
        this.RequestFrame();
      });
      window.addEventListener("pointermove", (e) => {
        this.onMouseMove(e);
        if (this.scene.processMouseMove(e, this.mouse, this.camera)) {
          this.RequestFrame();
        }
      });
      window.addEventListener("pointerup", (e) => {
        this.scene.processMouseUp(e);
        this.RequestFrame();
      });

      this.RequestFrame();
    });

    PubSub.subscribe("scene_animation_started", () => {
      this.EnableAnimationLoop();
    });

    PubSub.subscribe("scene_animation_finished", () => {
      this.DisableAnimationLoop();
    });

    PubSub.subscribe("scene_update_required", () => {
      this.RequestFrame();
    });
  }

  SetupHDR() {
    var env = undefined;

    var environmentTexture = new HDRCubeTextureLoader()
      .setPath("data/textures/hdri/Reinforced_Concrete_02/")
      .setDataType(THREE.FloatType)
      .load(
        ["px.hdr", "nx.hdr", "py.hdr", "ny.hdr", "pz.hdr", "nz.hdr"],
        () => {
          var pmremGenerator = new THREE.PMREMGenerator(this.renderer);
          pmremGenerator.compileCubemapShader();
          env = pmremGenerator.fromCubemap(environmentTexture);
          pmremGenerator.dispose();

          this.scene.scene.environment = env.texture;
          //this.scene.scene.background = env.texture;

          this.RequestFrame();
        }
      );
  }

  RequestFrame() {
    if (this.isRenderingActive) {
      return;
    }

    this.isRenderingActive = true;
    requestAnimationFrame(this.RenderFrame);
  }

  RenderFrame = () => {
    this.isRenderingActive = false;
    //this.renderer.render(this.scene.scene, this.camera);
    this.composer.render(this.clock.getDelta());
  };
}

var app = new App();
app.Init();
