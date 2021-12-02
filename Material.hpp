#pragma once
#include <glm/glm.hpp>

#include "Texture.hpp"

class Material
{
public:
	Texture& diffuse;
	Texture& specular;
	Texture& emission;
	float shininess;
};
