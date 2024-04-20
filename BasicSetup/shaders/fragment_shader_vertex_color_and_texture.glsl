#version 330 core
out vec4 FragColor;
in vec3 color;
in vec4 position;
in vec2 texPos;
uniform float horizontalOffset;
uniform sampler2D aTexture1;
uniform sampler2D aTexture2;

void main() {
	FragColor = mix(texture(aTexture1,texPos),texture(aTexture2,texPos),horizontalOffset);
}