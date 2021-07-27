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

    this.plane = new THREE.Plane(new THREE.Vector3(0, 0, 1), 0);
    //this.scene.add(this.plane);
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

          if (child.name === "Button_4") {
            console.log(child.userData.name);
          }

          if (child.material) {
            // if (child.material.name === "Steel.001") {
            //   console.log(child.material);
            //   child.material.normalScale = new Vector2(0.1, 0.1);
            //   child.material.normalMap.anisotropy = 16;
            //   child.material.needsUpdate = true;
            // }

            if (child.material.name === "LCD") {
              this.lcd = child.material;
              //   texture.needsUpdate = true;
              //   child.material.map.generateMipmaps = false;
              //child.material.roughness = 0.1;
              //child.material.metalness = 1;

              this.lcd.map = this.texture;
              this.lcd.map.format = THREE.RGBFormat;
              this.lcd.map.type = THREE.UnsignedShort565Type;
              this.lcd.map.minFilter = THREE.LinearFilter;
              this.lcd.map.magFilter = THREE.NearestFilter;
              this.lcd.map.wrapS = THREE.ClampToEdgeWrapping;
              this.lcd.map.wrapT !== THREE.ClampToEdgeWrapping;
              this.lcd.map.anisotropy = 16;
              this.update_lcd();
              //child.material.map.needsUpdate = true;
              //   child.material.color = new Three.Color(0xff0000);
            }

            console.log("LCD");
          }
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
      this.buttonPress = true;

      //console.log("BD: ", this.button_definitions);
      // console.log("ID: ", this.button_definitions[this.object.name]);
      if (this.button_definitions[this.object.name + "_DOWN"]) {
        this.firmwareBinder.Update(
          this.button_definitions[this.object.name + "_DOWN"]
        );
        this.update_lcd();
      }
    }

    if (name === "Knob_new") {
      this.mouseDownPosition.copy(mouse);

      this.knobDrag = true;
    }

    //animation.play();

    //.add({ targets: this.currentModel.rotation, x: rotationAngle })
    //.add({ targets: this.currentModel.position, y: heightOffset });
    //obj.visible = false;
  }

  processMouseUp(event) {
    if (this.object && this.buttonPress) {
      //console.log("MU: ", this.intersection.object.name);
      this.object.position.add(
        new THREE.Vector3(0.0, this.buttonPressDistance, 0.0)
      );

      //console.log("BB: ", this.object.name + "_UP");
      if (this.button_definitions[this.object.name + "_UP"]) {
        this.firmwareBinder.Update(
          this.button_definitions[this.object.name + "_UP"]
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

  processMouseMove(event, mouse) {
    //console.log("EX: ", event.movementX);
    // this.raycaster.ray.intersectPlane(this.plane, this.intersectPoint);
    // console.log("IP: ", this.intersectPoint);

    // this.knob.lookAt(0, this.intersectPoint.y, 0);
    if (this.object)
      if (this.object && this.knobDrag) {
        //var mouseX = event.clientX - container.clientWidth / 2;
        //console.log("MM: ", this.object);

        //var targetRotationX = mouse.x - this.mouseDownPosition.x;
        //if (isNaN(targetRotationX)) targetRotationX = 0;
        //console.log("M: ", mouse);
        //console.log("MO: ", this.mouseDownPosition);
        //console.log("ROT: ", targetRotationX);
        // var newAngle = Math.atan2(
        //   mouse.y - this.knobCenter.y,
        //   this.knobCenter.x - mouse.x
        // );

        var newAngle = Math.atan2(
          mouse.x - this.knobCenter.x,
          mouse.y - this.knobCenter.y
        );
        //newAngle += Math.PI / 2;

        if (this.oldAngle < 0) {
          this.oldAngle += this.fullCircleRad;
        }
        if (this.oldAngle > this.fullCircleRad) {
          fullRotations = this.oldAngle / this.fullCircleRad;
          this.oldAngle -= fullRotations * this.fullCircleRad;
        }
        console.log("NA: ", newAngle);

        var delta = this.newAngle + this.oldAngle;
        if (delta < 0) {
          // Underflow, convert to full circle
          delta += this.fullCircleRad;
        }
        if (delta > Math.PI) {
          // Convert from full circle (0..360) to half circle (-180..180) to get rotation direction
          delta -= this.fullCircleRad;
        }

        if (delta < -this.tick) {
          //value++;
          this.oldAngle += this.tick;
        } else if (delta > this.tick) {
          //value--;
          this.oldAngle -= this.tick;
        }
        console.log("ROT: ", this.oldAngle);

        this.object.rotation.y = -this.oldAngle; //targetRotationX / 100;
        this.oldAngle = newAngle;

        //console.log("MU: ", intersection.object.name);
        //intersection.object.position.add(
        //  new Three.Vector3(0.0, buttonPressDistance, 0.0)
        //);
        //intersection == null;
      }
  }
}
