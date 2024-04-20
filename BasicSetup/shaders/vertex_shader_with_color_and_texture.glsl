#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexPos;
uniform float horizontalOffset;
out vec3 color;
out vec4 position;
out vec2 texPos;


void main() {
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	color = aColor;
	texPos = aTexPos;
	position = vec4(aPos.x + horizontalOffset, aPos.y, aPos.z, 1.0);
}
