#version 330 core

uniform mat4 model;

in vec3 vertexColor;
in vec2 textureCoord;
in vec3 normal;
in vec3 fragmentPosition;

out vec4 fragmentColor;

uniform sampler2D ourTexture;

// light position is hard-coded for now
float lightIntensity = 5.0;
vec3 lightPosition = vec3(1, 3, 3);


void main() {
	vec3 worldSpaceNormal = mat3( transpose( inverse(model) ) ) * normal;
	vec3 lightDirection = normalize(lightPosition - fragmentPosition);

	float diffuseFactor = max(dot(worldSpaceNormal, lightDirection), 0.0);

	vec3 litColor = diffuseFactor * vec3(texture(ourTexture, textureCoord)); 

	fragmentColor = vec4(litColor, 1.0);
}