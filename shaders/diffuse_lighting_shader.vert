#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 textureCoordinates;
layout(location = 2) in vec3 colorValue;
layout(location = 3) in vec3 vertexNormal;

uniform mat4 MVP;

out vec3 vertexColor;
out vec2 textureCoord;
out vec3 normal;

void main() {
	vertexColor = colorValue;
	textureCoord = textureCoordinates;
	normal = vertexNormal;

	gl_Position = MVP * vec4(vertexPosition, 1);
	//gl_Position = vec4(vertexPosition, 1);
}
