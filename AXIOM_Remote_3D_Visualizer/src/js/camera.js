import * as THREE from "three/webgpu";
import { OrbitControls } from "three/examples/jsm/controls/OrbitControls";

export class Camera {
    constructor(renderContainer) {
        const aspectRatio = renderContainer.clientWidth / renderContainer.clientHeight;
        this.instance = new THREE.PerspectiveCamera(75, aspectRatio, 0.01, 1000);
        this.instance.position.set(-0.2, 0.5, 0.5);
        this.instance.setFocalLength(85);
        this.instance.updateProjectionMatrix();

        this.controls = null;
    }

    setupControls(renderer, target, onChange) {
        this.controls = new OrbitControls(this.instance, renderer.domElement);
        this.controls.target.copy(target);
        this.controls.mouseButtons = {
            MIDDLE: THREE.MOUSE.PAN,
            RIGHT: THREE.MOUSE.ROTATE,
        };
        this.controls.touches = {
            TWO: THREE.TOUCH.DOLLY_ROTATE,
        };
        this.controls.update();

        if (onChange) {
            this.controls.addEventListener("change", onChange);
        }
    }

    updateProjection(width, height) {
        this.instance.aspect = width / height;
        this.instance.updateProjectionMatrix();
    }
}
