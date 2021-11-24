#version 330 core

uniform mat4 model;
uniform vec3 cameraPosition;

in vec3 vertexColor;
in vec2 textureCoord;
in vec3 worldSpaceNormal;
in vec3 fragmentPosition;

out vec4 fragmentColor;

uniform sampler2D ourTexture;

// dummy fragment attributes
vec3 diffuseColor = texture(ourTexture, textureCoord).xyz;

// light parameters are hard-coded for now
float lightIntensity = 5.0;
vec3 lightPosition = vec3(0, 0, 0);
vec3 lightColor = vec3(1, 1, 1);
vec3 specColor = vec3(1, 1, 1);
float ambientStrength = 0;
float specularStrength = 5;

float materialShininess = 32;

void main() {
	vec3 lightDirection = normalize(lightPosition - fragmentPosition);

	vec3 viewDirection = normalize(cameraPosition - fragmentPosition);
	vec3 reflectDirection = reflect(-lightDirection, normalize(worldSpaceNormal));

	float spec = pow( max(dot(viewDirection, reflectDirection), 0.0), materialShininess);
	vec3 specular = specularStrength * spec * specColor;

	float diffuseFactor = max(dot(worldSpaceNormal, lightDirection), 0.0);
	vec3 ambient = ambientStrength * lightColor;

	vec3 litColor = (ambient + specular + diffuseFactor) * diffuseColor;

	fragmentColor = vec4(litColor, 1.0);
}
