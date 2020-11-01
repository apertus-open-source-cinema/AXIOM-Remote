#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D cameraPreviewTexture;
uniform float analogGain;

void main() { color = texture(cameraPreviewTexture, UV).xyz * analogGain; }