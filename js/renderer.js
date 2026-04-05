import * as THREE from 'three/webgpu'
import { bloom } from 'three/addons/tsl/display/BloomNode.js';
import { mix, mul, oneMinus, positionLocal, smoothstep, texture, time, rotateUV, Fn, uv, vec2, vec3, vec4, pass, texture3D, uniform, renderOutput, output, float, mrt, color } from 'three/tsl';

/**
 * Modern Renderer class using Three.js R183+ RenderPipeline and TSL.
 */
export class Renderer {
    constructor(canvas) {
        // Use WebGPURenderer which is the standard for the Node/TSL system in R183
        // It will automatically fall back to WebGL 2 if WebGPU is not supported
        this.instance = new THREE.WebGPURenderer({
            canvas: canvas,
            antialias: true,
            alpha: false,
            forceWebGL: false, // Set to true if you want to force WebGL 2 even if WebGPU is available
        });

        // Set size based on the actual container dimensions
        const width = canvas.clientWidth || window.innerWidth;
        const height = canvas.clientHeight || window.innerHeight;

        this.instance.setSize(width, height);
        this.instance.setPixelRatio(window.devicePixelRatio);
        this.instance.setClearColor(0x223344);

        // Tone Mapping and Color Space are handled by the RenderPipeline output node if configured
        this.instance.toneMapping = THREE.AgXToneMapping;
        this.instance.toneMappingExposure = 2.5;
        this.instance.outputColorSpace = THREE.SRGBColorSpace;

        this.instance.shadowMap.enabled = true;
        this.instance.shadowMap.type = THREE.PCFShadowMap;

        this.instance.init(); // Asynchronously start GPU initialization

        this.postProcessing = null;
        this.bloomNode = null;
        this.scenePass = null;

        this.bloomLayer = new THREE.Layers();
        this.bloomLayer.set(1);
    }

    /**
     * Set up the modern RenderPipeline with Selective Bloom.
     * @param {THREE.Scene} scene 
     * @param {THREE.Camera} camera 
     */
    async setupPostProcessing(scene, camera) {
        await this.instance.init();

        this.postProcessing = new THREE.PostProcessing(this.instance);

        // 1. Create the main scene pass
        this.scenePass = pass(scene, camera);

        // 2. Setup MRT for selective bloom
        this.scenePass.setMRT(mrt({
            output,
            bloomIntensity: float(0)
        }));

        // 3. Define the bloom effect: (beauty pass * intensity mask)
        // We get nodes from the scenePass after setup
        const beautyNode = this.scenePass.getTextureNode('output');
        const intensityNode = this.scenePass.getTextureNode('bloomIntensity');
        
        // Bloom only the parts with intensity > 0
        this.bloomNode = bloom(beautyNode.mul(intensityNode), 0.05, 0.1, 0.0);

        // 4. Final output: Combine original beauty (scenePass) with bloom result
        this.postProcessing.outputNode = this.scenePass.add(this.bloomNode).renderOutput();

        this.isReady = true;
    }

    /**
     * Mark objects for bloom by setting their MRT bloom intensity.
     */
    addBloomSelection(objects) {
        objects.forEach((obj) => {
            obj.layers.enable(1);
            obj.traverse((child) => {
                if (child.isMesh) {
                    child.layers.enable(1);
                    if (child.material) {
                        // Mark material to have full bloom intensity in Slot 1
                        child.material.mrtNode = mrt({
                            bloomIntensity: float(1)
                        });
                    }
                }
            });
        });
    }

    /**
     * Render the scene using the pipeline.
     */
    async render(scene, camera, delta) {
        if (this.isReady && this.postProcessing) {
            this.postProcessing.render();
        } else {
            // Fallback render while pipeline initializes
            this.instance.render(scene, camera);
        }
    }

    /**
     * Handle resize.
     */
    resize(width, height) {
        this.instance.setSize(width, height);
        // RenderPipeline handles size automatically via the renderer
    }
}
