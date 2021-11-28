#version 330 core

uniform mat4 model;
uniform vec3 cameraPosition;

in vec3 vertexColor;
in vec2 textureCoord;
in vec3 worldSpaceNormal;
in vec3 fragmentPosition;

out vec4 fragmentColor;

uniform sampler2D ourTexture;


// light parameters are hard-coded for now
struct Light {
	float intensity;
	vec3 position;
	vec3 color;
	float ambientIntensity;
	float attenuationFactor;
};

struct Material {
	vec3 diffuseColor;
	vec3 specularColor;
	vec3 ambientColor;
	float shininess;
};

vec4 litColor(Light light, Material material)
{
	vec3 lightDirection = normalize(light.position - fragmentPosition);
	vec3 viewDirection = normalize(cameraPosition - fragmentPosition);
	vec3 reflectDirection = reflect(-lightDirection, normalize(worldSpaceNormal));

	float specularFactor = pow( max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
	vec3 specularColor = specularFactor * material.specularColor;

	float diffuseFactor = max(dot(worldSpaceNormal, lightDirection), 0.0);

	float lightToFragmentDistance = distance(light.position, fragmentPosition);
	float lightAttenuation = 1 / (lightToFragmentDistance * lightToFragmentDistance) * light.attenuationFactor;



	vec3 litColor = (light.ambientIntensity + material.diffuseColor + specularColor) * light.color * diffuseFactor * lightAttenuation;
	return vec4(litColor, 1.0);
}

void main() {

	int arraySize = 2;
	Light light = Light(5.0, vec3(0, 3, -10), vec3(1, 1, 1), 0, 250);
	Material material = Material( texture(ourTexture, textureCoord).xyz, vec3(1, 1, 1), vec3(1, 1, 1), 32);

	fragmentColor = litColor(light, material);
}
