#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D cameraPreviewTexture;

void main() { color = texture(cameraPreviewTexture, UV).xyz * vec3(1, 1, 1); }