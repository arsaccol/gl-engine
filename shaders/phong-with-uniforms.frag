#version 330 core

uniform mat4 model;
uniform vec3 cameraPosition;

in vec3 vertexColor;
in vec2 textureCoord;
in vec3 worldSpaceNormal;
in vec3 fragmentPosition;

out vec4 fragmentColor;

uniform sampler2D ourTexture;

struct Light
{
	vec3 position;

	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
};

struct Material
{
	vec3 diffuse; // should be sampler2d?
	vec3 specular; // should be sampler2d?
	vec3 emission; // should be sampler2d?
	float shininess;
};

vec3 attenuate(vec3 litColor, vec3 fragmentPosition,  Light light)
{
	float fragLightDistance = distance(light.position, fragmentPosition);
	float attenuation = 1 / (fragLightDistance * fragLightDistance);

	vec3 attenuatedColor = litColor * attenuation * 100; // this last value shouldn't be hardcoded...

	return attenuatedColor;
}

vec3 enlightenFragment(Light light, Material material)
{
	vec3 lightDirection = normalize(light.position - fragmentPosition);
	vec3 viewDirection = normalize(cameraPosition - fragmentPosition);
	vec3 reflectDirection = reflect(-lightDirection, normalize(worldSpaceNormal));

	vec3 specular = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess) * light.specular;
	float diffuse = max(dot(worldSpaceNormal, lightDirection), 0.0);


	float fragLightDistance = distance(light.position, fragmentPosition);
	float attenuation = 1 / (fragLightDistance * fragLightDistance);

	vec3 litColor = (light.ambient + material.diffuse + specular) * light.diffuse * diffuse;

	return attenuate(litColor, fragmentPosition, light);
}


void main()
{
	Light redLight = Light( vec3(-5, 3, -5), vec3(1, 0, 0), vec3(0.05, 0.05, 0.05), vec3(1, 1, 1) );
	Light blueLight = Light( vec3(5, -3, 5), vec3(0, 0, 1), vec3(0.05, 0.05, 0.05), vec3(1, 1, 1) );
	Material material = Material(texture(ourTexture, textureCoord).rgb, vec3(0), vec3(0), 32);

	vec4 color = vec4(enlightenFragment(redLight, material), 1);
	color += vec4(enlightenFragment(blueLight, material), 1);

	fragmentColor = color;
}