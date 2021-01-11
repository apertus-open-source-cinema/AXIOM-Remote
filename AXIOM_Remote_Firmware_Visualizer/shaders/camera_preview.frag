#version 330 core

in vec2 UV;

out vec3 fragColor;

uniform sampler2D cameraPreviewTexture;
uniform float analogGain;
//uniform float desaturationFactor;
uniform float contrastFactor;

void main() 
{ 
	vec3 texture = texture (cameraPreviewTexture, UV) .xyz * analogGain;
    vec3 gray = vec3 (dot (texture, vec3 (0.2126, 0.7152, 0.0722)));
    fragColor = vec3 ((texture - 0.5) * contrastFactor + 0.5);
	//fragColor = vec3 (mix (texture, gray, desaturationFactor));
}
