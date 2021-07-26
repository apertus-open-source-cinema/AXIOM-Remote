"use strict";

import * as THREE from "three";
import { GLTFLoader } from "three/examples/jsm/loaders/GLTFLoader.js";

//var navigationCamera = null;
//var navigationCube = null;
var navigationWidth = 250;
var navigationHeight = 250;
var navigationOffset = 10;

export class NavigationCube {
  renderTexture = undefined;
  navigationScene = undefined;

  gltfLoader = new GLTFLoader();
  navigationCube = undefined;

  onMouseMove(event) {
    //   event.preventDefault();

    //   // calculate mouse position in normalized device coordinates
    //   // (-1 to +1) for both components

    mouse.x = ((event.clientX - navigationOffset) / navigationWidth) * 2 - 1;
    mouse.y =
      -(
        (event.clientY -
          container.height +
          navigationHeight +
          navigationOffset) /
        navigationHeight
      ) *
        2 +
      1;

    //   //mouse.x = ( event.clientX / container.clientWidth ) * 2 - 1;
    //   //mouse.y = - ( event.clientY / container.clientHeight ) * 2 + 1;
    //   // console.log(mouse);

    requestRenderIfNotRequested();
  }

  constructor() {
    this.navigationScene = new THREE.Scene();
    this.renderTexture = new THREE.WebGLRenderTarget(
      navigationWidth,
      navigationHeight,
      {
        format: THREE.RGBAFormat,
        minFilter: THREE.LinearFilter,
        magFilter: THREE.LinearMipMapLinearFilter,
      }
      //{ minFilter: THREE.LinearFilter, magFilter: THREE.NearestFilter }
    );

    this.SetupCamera();
    this.SetupLight();

    this.LoadModel("orientation_cube_v01.glb");
  }

  SetupCamera() {
    const size = 0.23;
    const near = 0;
    const far = 2;
    let aspectRatio = navigationWidth / navigationHeight;
    this.navigationCamera = new THREE.OrthographicCamera(
      (-aspectRatio * size) / 2,
      (aspectRatio * size) / 2,
      size / 2,
      -size / 2,
      near,
      far
    );
    // navigationCamera.zoom = 0.2;
    this.navigationCamera.position.set(0, 0, 1);
    this.navigationCamera.updateProjectionMatrix();
  }

  SetupLight() {
    var hemiLight = new THREE.HemisphereLight(0xffffff, 0x444444, 3);
    hemiLight.position.set(0, 300, 0);
    this.navigationScene.add(hemiLight);
  }

  AttachToContainer(container) {
    container.addEventListener("mousemove", onMouseMove, false);
  }

  LoadGLTF(filePath, callback) {
    this.gltfLoader.load(
      filePath,
      callback,
      function (xhr) {
        console.log(
          filePath + ": " + (xhr.loaded / xhr.total) * 100 + "% loaded"
        );
      },
      function (error) {
        console.error(error);
      }
    );
  }

  LoadModel(modelFilename) {
    let filePath = "data/models/" + modelFilename;

    this.LoadGLTF(filePath, (gltf) => {
      this.navigationCube = gltf.scene;

      this.navigationScene.add(this.navigationCube);
    });
  }

  get scene() {
    return navigationScene;
  }

  SetRotationFromCamera(camera) {
    this.navigationCube.rotation.setFromRotationMatrix(camera.matrix.invert());
  }

  Render(renderer) {
    renderer.setRenderTarget(this.renderTexture);
    renderer.setClearColor(0x000000, 0);
    renderer.clear();
    renderer.render(this.navigationScene, this.navigationCamera);
    renderer.setRenderTarget(null);
  }
}
