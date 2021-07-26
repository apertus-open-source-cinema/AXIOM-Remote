"use strict";

import * as THREE from "three";
import { OrbitControls } from "three/examples/jsm/controls/OrbitControls";
import { MainScene } from "./scenes/MainScene.js";

import PubSub from "pubsub-js";

export class App {
  renderContainer = undefined;
  uiContainer = undefined;
  scene = undefined;
  camera = undefined;
  controls = undefined;
  renderer = undefined;

  isRenderingActive = false;

  clock = new THREE.Clock();

  cameraTargetPosition = new THREE.Vector3(0, 0.0, 0);

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
      antialias: true,
      canvas: this.renderContainer,
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

  mouse = new THREE.Vector2();

  onMouseMove(event) {
    // calculate mouse position in normalized device coordinates
    // (-1 to +1) for both components
    this.mouse.x = (event.clientX / this.renderContainer.clientWidth) * 2 - 1;
    this.mouse.y = -(event.clientY / this.renderContainer.clientHeight) * 2 + 1;

    //console.log(this.mouse);
  }

  SetupScene() {
    this.scene = new MainScene();
    var token = PubSub.subscribe("scene_loaded", () => {
      window.addEventListener("pointerdown", (e) => {
        this.scene.processMouseDown(e, this.mouse, this.camera);
        this.RequestFrame();
      });
      window.addEventListener("pointermove", (e) => {
        this.onMouseMove(e);
        //     this.scene.processMouseMove(e, this.mouse);
        //     this.RequestFrame();
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

  // TODO: replace with real HDR
  SetupHDR() {
    var environmentTexture = new THREE.TextureLoader().load(
      "data/textures/hdri/reinforced_concrete_02.png"
    );
    environmentTexture.mapping = THREE.EquirectangularReflectionMapping;
    environmentTexture.encoding = THREE.sRGBEncoding;
    this.scene.scene.environment = environmentTexture;
  }

  UpdateSceneMaterial() {
    console.log("Update material");
    this.scene.UpdateMaterial();
    //this.RequestFrame();
  }

  Start() {
    requestAnimationFrame(this.RenderFrame);
  }

  RequestFrame() {
    //console.log("Requested frame");
    if (this.isRenderingActive) {
      return;
    }

    this.isRenderingActive = true;
    requestAnimationFrame(this.RenderFrame);
  }

  // TODO: Add delta time and smooth framerate
  RenderFrame = () => {
    //console.log("Render frame");
    this.isRenderingActive = false;

    if (this.animationEnabled) {
      const deltaTime = this.clock.getDelta();
      this.scene.animationMixer.update(deltaTime);
    }

    this.renderer.autoClear = false;
    this.renderer.clear();
    this.renderer.setViewport(
      0,
      0,
      this.renderContainer.clientWidth,
      this.renderContainer.clientHeight
    );
    this.renderer.render(this.scene.getScene, this.camera);
  };
}

var app = new App();
app.Init();
