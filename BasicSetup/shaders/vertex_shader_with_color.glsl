#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
uniform float horizontalOffset;
out vec3 color;
out vec4 position;

void main() {
	gl_Position = vec4(aPos.x + horizontalOffset, aPos.y, aPos.z, 1.0);
	color = aColor;
	position = vec4(aPos.x + horizontalOffset, aPos.y, aPos.z, 1.0);
}
