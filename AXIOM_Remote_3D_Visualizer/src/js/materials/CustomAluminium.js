"use strict";

import { ShaderMaterial } from "../three.js/three.module.js";

const vertexShader =
  "attribute vec4 a_position; \
uniform vec4 u_offset; \
void main() { \
   gl_Position = a_position + u_offset; \
}";
const fragmentShader =
  "precision mediump float; \
uniform sampler2D u_texture; \
void main() { \
   vec2 texcoord = vec2(0.5, 0.5)  // get a value from the middle of the texture \
   gl_FragColor = texture2D(u_texture, texcoord); \
}";

export const CustomAluminiumMaterial = new ShaderMaterial({
  vertexShader: vertexShader,
  fragmentShader: fragmentShader,
});
