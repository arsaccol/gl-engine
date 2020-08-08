#version 330 core

in vec3 vertexColor;
out vec3 fragmentColor;

void main() {
	fragmentColor = vertexColor;
}
