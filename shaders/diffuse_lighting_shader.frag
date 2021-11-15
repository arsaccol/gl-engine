#version 330 core

in vec3 vertexColor;
in vec2 textureCoord;
in vec3 normal;

out vec4 fragmentColor;

uniform sampler2D ourTexture;

void main() {
	//fragmentColor = vertexColor;
	// just do some random calculation using the normal to make sure it's being passed 
	// the part after the multiplication symbol has no real logic, I just made something up
	fragmentColor = texture(ourTexture, textureCoord) * length(normal + vec3(1.0, 0.1, 0.1));
}