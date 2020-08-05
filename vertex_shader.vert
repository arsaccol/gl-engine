#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 textureCoordinates;
layout(location = 2) in vec3 colorValue;

//uniform mat4 mvp;

out vec4 vertexColor;

void main() {

	gl_Position = vec4(vertexPosition_modelspace, 1);

	//gl_Position.w = 1.0;
}