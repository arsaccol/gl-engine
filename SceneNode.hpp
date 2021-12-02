#pragma once

#include <vector>
#include <memory>
#include <algorithm> // maybe unneeded?
#include <glm/glm.hpp>
#include "Transform.hpp"

#include "entt/entt.hpp"

// Scene nodes only contain 

struct SceneNode
{
	entt::entity parent;
	std::vector<entt::entity> children;

	void applyTransform(const Transform& transform)
	{

	}
};

