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
struct Light {
	float intensity;
	vec3 position;
	vec3 color;
	float ambientIntensity;
};

struct Material {
	sampler2D diffuseColor;
	vec3 specularColor;
	vec3 ambientColor;
	float shininess;
};

// material properties I suppose?
vec3 specColor = vec3(1, 1, 1);
float materialShininess = 32;

//vec3 litColor(Light light, Material material)
//{
//	return vec3(1, 1, 1);
//}

void main() {

	Light light = Light(5.0, vec3(0, 0, 0), vec3(1, 1, 1), 0);

	vec3 lightDirection = normalize(light.position - fragmentPosition);

	vec3 viewDirection = normalize(cameraPosition - fragmentPosition);
	vec3 reflectDirection = reflect(-lightDirection, normalize(worldSpaceNormal));

	float spec = pow( max(dot(viewDirection, reflectDirection), 0.0), materialShininess);
	vec3 specular = spec * specColor;

	float diffuseFactor = max(dot(worldSpaceNormal, lightDirection), 0.0);
	vec3 ambient = light.ambientIntensity * light.color;

	float lightToFragmentDistance = distance(light.position, fragmentPosition);
	float lightAttenuation = 1 / (lightToFragmentDistance * lightToFragmentDistance) * 100;

	vec3 litColor = (light.ambientIntensity + diffuseColor + specular) * light.color * diffuseFactor * lightAttenuation;

	fragmentColor = vec4(litColor, 1.0);
}
