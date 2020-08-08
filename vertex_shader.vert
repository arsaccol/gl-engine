#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 textureCoordinates;
layout(location = 2) in vec3 colorValue;

//uniform mat4 mvp;

out vec3 vertexColor;

void main() {
	vertexColor = colorValue;

	gl_Position = vec4(vertexPosition * 1, 1);
}