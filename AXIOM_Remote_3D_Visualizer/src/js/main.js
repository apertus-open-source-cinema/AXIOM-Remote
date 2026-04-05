"use strict";

import * as THREE from "three/webgpu";
import { HDRCubeTextureLoader } from "three/examples/jsm/loaders/HDRCubeTextureLoader.js";
import eventBus from "./eventBus.js";
import { Camera } from "./camera.js";
import { Renderer } from "./renderer.js";
import { InputManager } from "./inputManager.js";
import { MainScene } from "./scenes/MainScene.js";

export class App {
  clock = new THREE.Timer();
  isRenderingActive = false;

  cameraTargetPosition = new THREE.Vector3(0.0, 0.0, 0.0);

  Init() {
    this.appContainer = document.getElementById("app");

    this.canvas = document.getElementById("render_canvas");
    this.renderContainer = this.canvas.parentElement;

    const width = this.renderContainer.clientWidth;
    const height = this.renderContainer.clientHeight;

    this.scene = new MainScene();
    this.camera = new Camera(this.renderContainer);
    this.renderer = new Renderer(this.canvas);

    this.camera.setupControls(this.renderer.instance, this.cameraTargetPosition, () => this.RequestFrame());
    
    // Initializing post-processing asynchronously
    this.renderer.setupPostProcessing(this.scene.scene, this.camera.instance).then(() => {
        this.RequestFrame();
    });

    this.input = new InputManager(this.renderContainer, this.scene.scene, this.camera.instance);

    this.SetupHDR();

    this.SetupSubscriptions();

    // Trigger initial resize to ensure canvas fills container
    this.HandleResize();
  }

  SetupSubscriptions() {
    eventBus.on("scene_loaded", () => {
      this.renderer.addBloomSelection(this.scene.glowingObjects);
      this.RequestFrame();
    });

    eventBus.on("scene_update_required", () => this.RequestFrame());

    // window.addEventListener('resize', () => this.HandleResize());
    this.resizeObserver = new ResizeObserver(entries => {
      console.log("Resized")
      const rect = entries[0].contentRect;
      this.renderer.resize(rect.width, rect.height);
      this.camera.updateProjection(rect.width, rect.height);
      // this.RequestFrame();
      this.renderer.render(this.scene.scene, this.camera.instance, 0);
    });

    this.resizeObserver.observe(this.renderContainer);
  }

  HandleResize() {
    const width = this.renderContainer.clientWidth;
    const height = this.renderContainer.clientHeight;
    
    this.renderer.resize(width, height);
    this.camera.updateProjection(width, height);
    this.RequestFrame();
  }

  SetupHDR() {
    let env = undefined;

    const environmentTexture = new HDRCubeTextureLoader()
      .setPath("data/textures/hdri/Reinforced_Concrete_02/")
      .setDataType(THREE.FloatType)
      .load(
        ["px.hdr", "nx.hdr", "py.hdr", "ny.hdr", "pz.hdr", "nz.hdr"],
        (environmentTexture) => {
          environmentTexture.colorSpace = THREE.SRGBColorSpace;
          this.scene.scene.environment = environmentTexture;
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

  // RenderFrame = () => {
  //   this.isRenderingActive = false;
  //   if (this.scene) this.scene.update_lcd();
  //   this.renderer.render(this.scene.scene, this.camera.instance, this.clock.getDelta());
  //   this.RequestFrame();
  // };

  RenderFrame = () => {
    requestAnimationFrame(this.RenderFrame);

    const delta = this.clock.getDelta();

    if (this.scene) this.scene.update_lcd();
    this.renderer.render(this.scene.scene, this.camera.instance, delta);
  };
}

var app = new App();
app.Init();
