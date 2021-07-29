"use strinct";

import * as THREE from "three";
import { GLTFLoader } from "three/examples/jsm/loaders/GLTFLoader.js";
import anime from "animejs";
import PubSub from "pubsub-js";
import axiomRemoteFirmware from "../FW/axiom_remote_firmware.js";

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

  texture = null;
  image_data = null;

  plane = null;
  intersectPoint = new THREE.Vector3();

  m = undefined;
  firmwareBinder = undefined;

  mousePoint = undefined;
  intersectPoint = undefined;

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

    const firmware = axiomRemoteFirmware({
      locateFile(path) {
        if (path.endsWith(".wasm")) {
          return "js/" + path;
        }
        return path;
      },
    });

    firmware.then((Module) => {
      this.m = Module;
      console.log("FW: ", this.m);

      this.SetupDefinitions();

      this.firmwareBinder = new this.m.FirmwareBinder();
      this.image_data = this.firmwareBinder.GetFB();
      this.texture = new THREE.DataTexture(
        this.image_data,
        320,
        240,
        THREE.RGBAFormat
      );
    });

    this.SetupLight();
    this.LoadModel("../../data/models/axiom_remote02.glb");

    // const geometry = new THREE.PlaneGeometry(0, 0, 1);
    // const material = new THREE.MeshBasicMaterial({
    //   color: 0xffff00,
    //   side: THREE.DoubleSide,
    // });
    // this.plane = new THREE.Mesh(geometry, material);
    this.planeZ = new THREE.Plane(new THREE.Vector3(0, 1, 0), 0);
    //    this.scene.add(this.plane);

    var sphere = new THREE.SphereBufferGeometry(0.0005, 16, 8);
    this.mousePoint = new THREE.Mesh(
      sphere,
      new THREE.MeshBasicMaterial({ color: "red" })
    );

    this.scene.add(this.mousePoint);

    this.intersectPoint = new THREE.Vector3();
  }

  get getScene() {
    return this.scene;
  }

  SetupLight() {
    // remember the axis are:
    // X right
    // Y up
    // Z back

    const light = new THREE.AmbientLight(0x444444); // soft white light
    //this.scene.add(light);

    // Spotlight 1
    const light1 = new THREE.SpotLight(
      0xaaaaff,
      1.3,
      1,
      THREE.MathUtils.degToRad(45),
      0.3
    );
    light1.position.set(0.4, 0.4, 0.3);

    //Set up shadow properties for the light
    light1.castShadow = true;
    light1.shadow.mapSize.width = 2048; // default
    light1.shadow.mapSize.height = 2048; // default
    light1.shadow.camera.near = 0.1; // default
    light1.shadow.camera.far = 0.6; // default
    light1.shadow.radius = 8;
    light1.shadow.bias = -0.001;
    this.scene.add(light1);

    // Spotlight 2
    const light2 = new THREE.SpotLight(
      0xffbb66,
      1.3,
      1,
      THREE.MathUtils.degToRad(45),
      0.3
    );
    light2.position.set(-0.4, 0.45, -0.2);

    //Set up shadow properties for the light
    light2.castShadow = true;
    light2.shadow.mapSize.width = 2048; // default
    light2.shadow.mapSize.height = 2048; // default
    light2.shadow.camera.near = 0.1; // default
    light2.shadow.camera.far = 0.6; // default
    light2.shadow.radius = 8;
    light2.shadow.bias = -0.001;
    this.scene.add(light2);

    const width = 1;
    const height = 1;
    const intensity = 1;
    const rectLight = new THREE.RectAreaLight(
      0x444444,
      intensity,
      width,
      height
    );
    rectLight.position.set(0.4, 0.2, 0);
    rectLight.lookAt(0, 0, 0);
    this.scene.add(rectLight);
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

  knob = undefined;
  knobCenter = new THREE.Vector2();

  button_definitions = undefined;

  lcd = undefined;

  update_lcd() {
    console.log("Update LCD");
    //this.image_data = this.firmwareBinder.UpdateFB();
    this.texture.needsUpdate = true;
    // this.lcd.map.needsUpdate = true;
  }

  // TODO: Check if this can be simplified
  SetupDefinitions() {
    this.button_definitions = {
      Button_1_DOWN: this.m.ButtonID.BUTTON_1_DOWN,
      Button_1_UP: this.m.ButtonID.BUTTON_1_UP,
      Button_2_DOWN: this.m.ButtonID.BUTTON_2_DOWN,
      Button_2_UP: this.m.ButtonID.BUTTON_2_UP,
      Button_3_DOWN: this.m.ButtonID.BUTTON_3_DOWN,
      Button_3_UP: this.m.ButtonID.BUTTON_3_UP,
      Button_4_DOWN: this.m.ButtonID.BUTTON_4_DOWN,
      Button_4_UP: this.m.ButtonID.BUTTON_4_UP,
      Button_5_DOWN: this.m.ButtonID.BUTTON_5_DOWN,
      Button_5_UP: this.m.ButtonID.BUTTON_5_UP,
      Button_6_DOWN: this.m.ButtonID.BUTTON_6_DOWN,
      Button_6_UP: this.m.ButtonID.BUTTON_6_UP,
      Button_7_DOWN: this.m.ButtonID.BUTTON_7_DOWN,
      Button_7_UP: this.m.ButtonID.BUTTON_7_UP,
      Button_8_DOWN: this.m.ButtonID.BUTTON_8_DOWN,
      Button_8_UP: this.m.ButtonID.BUTTON_8_UP,
      Button_9_DOWN: this.m.ButtonID.BUTTON_9_DOWN,
      Button_9_UP: this.m.ButtonID.BUTTON_9_UP,
      Button_10_DOWN: this.m.ButtonID.BUTTON_10_DOWN,
      Button_10_UP: this.m.ButtonID.BUTTON_10_UP,
      Button_11_DOWN: this.m.ButtonID.BUTTON_11_DOWN,
      Button_11_UP: this.m.ButtonID.BUTTON_11_UP,
      Button_12_DOWN: this.m.ButtonID.BUTTON_12_DOWN,
      Button_12_UP: this.m.ButtonID.BUTTON_12_UP,
      Knob_new_DOWN: this.m.ButtonID.E1_DOWN,
      Knob_new_UP: this.m.ButtonID.E1_UP,
      BUTTON_NONE: this.m.ButtonID.BUTTON_NONE,
    };
  }

  glowingObjects = [];

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
      var width = -boundingBox.max.x - boundingBox.min.x;
      var length = boundingBox.max.z + boundingBox.min.z;
      gltfScene.position.set(width / 2, 0, length / 2);

      //const axis = new THREE.AxesHelper();
      //this.scene.add(axis);

      // Add bounding box visualisation
      //const box = new THREE.BoxHelper(gltfScene, 0xffff00);
      //this.scene.add(box);

      gltfScene.traverse((child) => {
        if (child.name === "Knob_new") {
          this.knob = child;
          console.log(this.knob);
          //const axis = new THREE.AxesHelper(0.1);
          var x = this.knob.position.x + width / 2;
          //axis.position.set(x, this.knob.position.y, this.knob.position.z);
          //this.scene.add(axis);

          this.knobCenter.x = x;
          this.knobCenter.y = this.knob.position.z;
        }

        if (child.name.startsWith("Button_") || child.name.startsWith("LED")) {
          this.glowingObjects.push(child);
        }

        if (child.isMesh) {
          child.castShadow = true;
          child.receiveShadow = true;

          if (child.material) {
            if (child.material.name === "LCD") {
              this.lcd = child.material;
              this.lcd.map = this.texture;
              this.lcd.map.format = THREE.RGBFormat;
              this.lcd.map.type = THREE.UnsignedShort565Type;
              this.lcd.map.minFilter = THREE.LinearFilter;
              this.lcd.map.magFilter = THREE.NearestFilter;
              this.lcd.map.wrapS = THREE.ClampToEdgeWrapping;
              this.lcd.map.wrapT !== THREE.ClampToEdgeWrapping;
              this.lcd.map.anisotropy = 16;
              this.update_lcd();
            }
          }
        }
      });

      this.scene.add(gltfScene);
    });
  }

  raycaster = new THREE.Raycaster();
  intersection = null;
  mouseDownPosition = new THREE.Vector2();
  buttonPressDistance = 0.001;
  buttonPress = false;
  knobDrag = false;

  object = null;

  processMouseDown(event, mouse, camera) {
    if (event.which != 1) {
      return;
    }

    this.raycaster.setFromCamera(mouse, camera);

    var intersects = this.raycaster.intersectObjects(this.scene.children, true);
    if (intersects[0] === undefined) {
      return;
    }

    this.intersection = intersects[0];
    var name = this.intersection.object.name;

    //obj = intersection.object;
    console.log("MD: ", intersects);

    this.object = this.intersection.object;
    if (name.startsWith("Button") || name === "Knob_press") {
      //console.log("MD: ", name);

      //var obj = intersection.object;
      if (name.startsWith("Knob_press")) {
        this.object = this.object.parent;
      }

      this.object.position.add(
        new THREE.Vector3(0.0, -this.buttonPressDistance, 0.0)
      );

      if (this.button_definitions[this.object.name + "_DOWN"]) {
        console.log("MMMDDD");
        this.firmwareBinder.Update(
          this.button_definitions[this.object.name + "_DOWN"],
          this.knobValue
        );
        this.update_lcd();
      }
    }

    this.buttonPress = true;

    if (name === "Knob_new") {
      this.knobDrag = true;
    }
  }

  processMouseUp(event) {
    if (this.object && this.buttonPress) {
      this.object.position.add(
        new THREE.Vector3(0.0, this.buttonPressDistance, 0.0)
      );

      if (this.button_definitions[this.object.name + "_UP"]) {
        this.firmwareBinder.Update(
          this.button_definitions[this.object.name + "_UP"],
          this.knobValue
        );
        this.update_lcd();
      }

      this.intersection = null;
      this.buttonPress = false;
      this.object = null;
    }

    if (this.knobDrag === true) {
      console.log("Drag END");
      this.knobDrag = false;
      this.object = null;
    }
  }

  fullCircleRad = 2 * Math.PI;
  tick = this.fullCircleRad / 12; // 30°, 12 -> possible detents of real rotary encoder
  oldAngle = 0.0;
  knobAngle = 0.0;
  knobValue = 0;

  processMouseMove(event, mouse, camera) {
    if (this.object && this.knobDrag) {
      this.raycaster.setFromCamera(mouse, camera);
      this.raycaster.ray.intersectPlane(this.planeZ, this.intersectPoint);

      this.mousePoint.position.x = this.intersectPoint.x;
      this.mousePoint.position.y = this.intersectPoint.y;
      this.mousePoint.position.z = this.intersectPoint.z;

      var newAngle = Math.atan2(
        this.knobCenter.y - this.intersectPoint.z,
        this.intersectPoint.x - this.knobCenter.x
      );
      newAngle -= Math.PI / 2;

      if (this.oldAngle < 0) {
        this.oldAngle += this.fullCircleRad;
      }
      if (this.oldAngle > this.fullCircleRad) {
        var fullRotations = this.oldAngle / this.fullCircleRad;
        this.oldAngle -= fullRotations * this.fullCircleRad;
      }

      var delta = newAngle + this.oldAngle;
      if (delta < 0) {
        // Underflow, convert to full circle
        delta += this.fullCircleRad;
      }
      if (delta > Math.PI) {
        // Convert from full circle (0..360) to half circle (-180..180) to get rotation direction
        delta -= this.fullCircleRad;
      }

      if (delta < -this.tick) {
        this.knobValue--;
        this.oldAngle += this.tick;
      } else if (delta > this.tick) {
        this.knobValue++;
        this.oldAngle -= this.tick;
      }

      this.object.rotation.y = -this.oldAngle;

      if (this.knobValue != 0) {
        this.firmwareBinder.Update(
          this.button_definitions["BUTTON_NONE"],
          this.knobValue
        );
        this.update_lcd();

        this.knobValue = 0;

        return true;
      }

      return false;
    }
  }
}
