import * as THREE from "three";
import PubSub from "pubsub-js";

export class InputManager {
    constructor(renderContainer, scene, camera) {
        this.renderContainer = renderContainer;
        this.scene = scene;
        this.camera = camera;
        this.raycaster = new THREE.Raycaster();
        this.mouse = new THREE.Vector2();

        this.activeObject = null;
        this.isButtonDown = false;
        this.isKnobDragging = false;
        this.buttonPressDistance = 0.001;

        window.addEventListener("pointerdown", (e) => this.onPointerDown(e));
        window.addEventListener("pointermove", (e) => this.onPointerMove(e));
        window.addEventListener("pointerup", () => this.onPointerUp());
    }

    updateMousePosition(event) {
        this.mouse.x = ((event.clientX - this.renderContainer.offsetLeft) / this.renderContainer.clientWidth) * 2 - 1;
        this.mouse.y = -(event.clientY / this.renderContainer.clientHeight) * 2 + 1;
    }

    onPointerDown(event) {
        if (event.which !== 1) return;
        this.updateMousePosition(event);

        this.raycaster.setFromCamera(this.mouse, this.camera);
        const intersects = this.raycaster.intersectObjects(this.scene.children, true);

        if (intersects.length === 0) return;

        const object = intersects[0].object;
        const name = object.name;

        if (name.startsWith("Button") || name === "Knob_press") {
            this.activeObject = name === "Knob_press" ? object.parent : object;
            this.handleButtonInteraction(this.activeObject, 'DOWN');
        } else if (name === "Knob") {
            this.activeObject = object;
            this.isKnobDragging = true;
        }
    }

    onPointerMove(event) {
        this.updateMousePosition(event);

        if (this.isKnobDragging && this.activeObject) {
            PubSub.publish("knob_move", { mouse: this.mouse, camera: this.camera });
        }
    }

    onPointerUp() {
        if (this.activeObject && this.isButtonDown) {
            this.handleButtonInteraction(this.activeObject, 'UP');
        }
        this.isKnobDragging = false;
        this.activeObject = null;
    }

    handleButtonInteraction(obj, state) {
        const move = state === 'DOWN' ? -this.buttonPressDistance : this.buttonPressDistance;
        obj.position.y += move;
        this.isButtonDown = state === 'DOWN';

        PubSub.publish("input_interaction", { name: obj.name, state });
        PubSub.publish("scene_update_required");
    }
}
