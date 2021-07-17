#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 textureCoordinates;
layout(location = 2) in vec3 colorValue;

uniform mat4 MVP;

out vec3 vertexColor;
out vec2 textureCoord;

void main() {
	vertexColor = colorValue;
	textureCoord = textureCoordinates;

	gl_Position = MVP * vec4(vertexPosition, 1);
	//gl_Position = vec4(vertexPosition, 1);
}
