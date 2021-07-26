"use strinct";

import * as THREE from "../three.js/three.module.js";
import { GLTFLoader } from "../three.js/loaders/GLTFLoader.js";
import anime from "../anime.es.js";
import "../pubsub.js";

// TODO: Place LoadingManager properly, currently it's just for testing purposes here
const manager = new THREE.LoadingManager();
manager.onStart = function (url, itemsLoaded, itemsTotal) {
  console.log(
    "Started loading file: " +
      url +
      ".\nLoaded " +
      itemsLoaded +
      " of " +
      itemsTotal +
      " files."
  );
};

manager.onProgress = function (url, itemsLoaded, itemsTotal) {
  console.log(
    "Loading file: " +
      url +
      ".\nLoaded " +
      itemsLoaded +
      " of " +
      itemsTotal +
      " files."
  );
};

manager.onError = function (url) {
  console.log("There was an error loading " + url);
};

export class MainScene {
  scene = undefined;
  lightTargetPosition = new THREE.Vector3(0.1, 0, -0.1);
  gltfLoader = new GLTFLoader(manager);

  currentModel = undefined;
  currentAnimations = undefined;

  animationMixer = undefined;

  runningAnimations = 0;

  material = {
    color: 0xb1b1b1,
    roughness: 0.2,
    metalness: 0.9,
    reflectivity: 0,
    clearcoat: 0.6,
    clearcoatRoughness: 0.6,
  };

  constructor() {
    this.scene = new THREE.Scene();

    manager.onLoad = function () {
      console.log("Loading complete!");
      PubSub.publishSync("scene_loaded");
    };

    this.SetupLight();
    this.LoadEnvironment();
    //this.LoadModel("cap_bottom_v5.glb");
    this.LoadModel("assembly_animation_test_V02.glb");
  }

  get getScene() {
    return this.scene;
  }

  SetupLight() {
    // remember the axis are:
    // X right
    // Y up
    // Z back

    // Spotlight 1
    const light1 = new THREE.SpotLight(
      0xffffff,
      5,
      2,
      THREE.MathUtils.degToRad(13),
      0.3
    );
    light1.position.set(0.4, 0.4, 0.7);

    light1.target.position.copy(this.lightTargetPosition);
    this.scene.add(light1.target);

    //Set up shadow properties for the light
    light1.castShadow = true;
    light1.shadow.mapSize.width = 2048; // default
    light1.shadow.mapSize.height = 2048; // default
    light1.shadow.camera.near = 0.5; // default
    light1.shadow.camera.far = 2; // default
    light1.shadow.radius = 8;

    this.scene.add(light1);
    light1.shadow.bias = -0.001;

    // Spotlight 2
    const light2 = new THREE.SpotLight(
      0xffffff,
      10,
      2,
      THREE.MathUtils.degToRad(15),
      0.3
    );
    light2.position.set(-0.4, 0.45, -0.2);

    light2.target.position.copy(this.lightTargetPosition);
    this.scene.add(light2.target);

    //Set up shadow properties for the light
    light2.castShadow = true;
    light2.shadow.mapSize.width = 2048; // default
    light2.shadow.mapSize.height = 2048; // default
    light2.shadow.camera.near = 0.5; // default
    light2.shadow.camera.far = 2; // default
    light2.shadow.radius = 8;
    light2.shadow.bias = -0.001;

    this.scene.add(light2);

    // Spotlight 3
    const light3 = new THREE.SpotLight(
      0xffffff,
      5,
      3,
      THREE.MathUtils.degToRad(5),
      0.5
    );
    light3.position.set(0.8, 0.4, -2);
    light3.target.position.copy(this.lightTargetPosition);
    this.scene.add(light3.target);

    //Set up shadow properties for the light
    light3.castShadow = true;
    light3.shadow.mapSize.width = 2048; // default
    light3.shadow.mapSize.height = 2048; // default
    light3.shadow.camera.near = 0.5; // default
    light3.shadow.camera.far = 2; // default
    light3.shadow.radius = 8;
    light3.shadow.bias = -0.001;

    this.scene.add(light3);
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

  LoadEnvironment() {
    let filePath = "data/models/environment/environment_V01.gltf";

    this.LoadGLTF(filePath, (gltf) => {
      const gltfScene = gltf.scene;

      gltfScene.traverse(function (child) {
        if (child.isMesh) {
          console.info("Enable shadow cast/receive");
          child.castShadow = true;
          child.receiveShadow = true;

          if (child.material.map) {
            child.material.map.anisotropy = 16;
          }
        }
      });

      this.scene.add(gltfScene);
    });
  }

  LoadModel(modelFilename) {
    let filePath = "data/models/" + modelFilename;

    this.LoadGLTF(filePath, (gltf) => {
      const gltfScene = gltf.scene;
      this.currentModel = gltf.scene;
      this.currentAnimations = gltf.animations;
      this.animationMixer = new THREE.AnimationMixer(this.currentModel);
      this.animationMixer.addEventListener("finished", () => {
        //console.log(this.currentAnimations);
        this.runningAnimations--;
        console.log("runningAnimations: " + this.runningAnimations);
        if (this.runningAnimations === 0) {
          PubSub.publishSync("scene_animation_finished");
        }
      });

      var boundingBox = new THREE.Box3().setFromObject(gltfScene);
      //if (alignOrigin) {
      var width = Math.abs(boundingBox.min.x);
      var height = Math.abs(boundingBox.min.y);
      var length = Math.abs(boundingBox.max.z);
      gltfScene.position.set(width, height, -length);
      //}

      // Add bounding box visualisation
      //const box = new THREE.BoxHelper(gltfScene, 0xffff00);
      //this.scene.add(box);

      gltfScene.traverse((child) => {
        if (child.isMesh) {
          child.castShadow = true;
          child.receiveShadow = true;
          //     if (child.material.map) {
          //       child.material.map.anisotropy = 16;
          //     }

          // assign metal material to object
          const aluminummaterial = new THREE.MeshPhysicalMaterial({
            color: this.material.color,
          });
          aluminummaterial.roughness = this.material.roughness;
          aluminummaterial.metalness = this.material.metalness;
          aluminummaterial.reflectivity = this.material.reflectivity;
          aluminummaterial.clearcoat = this.material.clearcoat;
          aluminummaterial.clearcoatRoughness = this.material.clearcoatRoughness;

          child.material = aluminummaterial;
        }
      });

      this.scene.add(gltfScene);
    });
  }

  FlipModel = () => {
    var boundingBox = new THREE.Box3().setFromObject(this.currentModel);
    var rotation = this.currentModel.rotation.x;
    var rotationAngle = 0;
    var heightOffset = -boundingBox.min.y;

    if (rotation == 0) {
      rotationAngle = THREE.Math.degToRad(180);
      heightOffset = boundingBox.max.y;
    }

    anime
      .timeline({
        easing: "linear",
        duration: 200,
        begin: () => PubSub.publishSync("scene_animation_started"),
        complete: () => PubSub.publishSync("scene_animation_finished"),
      })
      .add({ targets: this.currentModel.position, y: boundingBox.max.y * 2 })
      .add({ targets: this.currentModel.rotation, x: rotationAngle })
      .add({ targets: this.currentModel.position, y: heightOffset });
  };

  get currentMaterial() {
    return this.material;
  }

  UpdateMaterial() {
    this.currentModel.children.forEach((element) => {
      if (element.isMesh) {
        element.material.color.set(this.material.color);
        element.material.roughness = this.material.roughness;
        element.material.metalness = this.material.metalness;
        element.material.reflectivity = this.material.reflectivity;
        element.material.clearcoat = this.material.clearcoat;
        element.material.clearcoatRoughness = this.material.clearcoatRoughness;
      }
    });

    PubSub.publishSync("scene_update_required");
  }

  PlayAnimation(index) {
    var action = this.animationMixer
      .clipAction(this.currentAnimations[index])
      .setLoop(THREE.LoopOnce);
    action.clampWhenFinished = true;
    action.stop();
    action.reset();
    this.runningAnimations++;

    PubSub.publishSync("scene_animation_started");
    action.play();
  }
}
