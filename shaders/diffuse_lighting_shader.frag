#version 330 core

in vec3 vertexColor;
in vec2 textureCoord;
out vec4 fragmentColor;

uniform sampler2D ourTexture;

void main() {
	//fragmentColor = vertexColor;
	fragmentColor = texture(ourTexture, textureCoord);
}