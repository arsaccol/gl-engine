#pragma once 

#include <entt/entt.hpp>
#include <memory>
#include "../Mesh.hpp"
#include "../Texture.hpp"
#include "../Transform.hpp"
#include "../ResourceManager.hpp"
#include "../SceneNode.hpp"
#include "../Light.hpp" // Added for Light component

struct Factory
{
	// Factory functions rely on resources having already been loaded
	Factory()
		: textureManager{ ResourceManager::textureManager },
		meshManager{ResourceManager::meshManager}
	{

	}


	entt::entity Human(entt::registry& registry, entt::entity parent)
	{
		entt::entity humanEntity = registry.create();
		registry.emplace<Transform>(humanEntity);
		registry.emplace<SceneNode>(humanEntity);
		SceneNode::addChild(registry, parent, humanEntity);
		

		registry.emplace<std::shared_ptr<Mesh>>(humanEntity, meshManager["human"]);
		registry.emplace<std::shared_ptr<Texture>>(humanEntity, textureManager["human"]);

		return humanEntity;
	}

	entt::entity BlenderCube(entt::registry& registry, entt::entity parent)
	{
		entt::entity cubeEntity = registry.create();
		registry.emplace<SceneNode>(cubeEntity);
		SceneNode::addChild(registry, parent, cubeEntity);

		registry.emplace<Transform>(cubeEntity, 
			glm::vec3{
				static_cast<float>(rand() % 50 - 25) + 10,
				static_cast<float>(rand() % 50 - 25),
				static_cast<float>(rand() % 50 - 25),
			}
		);

		registry.emplace<std::shared_ptr<Mesh>>(cubeEntity, meshManager["cube"]);
		registry.emplace<std::shared_ptr<Texture>>(cubeEntity, textureManager["cube"]);

		return cubeEntity;
	}

	entt::entity createLight(
		entt::registry& registry,
		entt::entity parent,
		const Light& lightProperties,
		const glm::vec3& position = glm::vec3(0.0f)
		// Scale and orientation are not directly settable via current Transform constructor
		// const glm::vec3& scale = glm::vec3(1.0f),
		// const glm::quat& orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f)
	)
	{
		entt::entity lightEntity = registry.create();

		// Add Transform component (using constructor that takes position)
		registry.emplace<Transform>(lightEntity, position);
		// If scale/orientation need to be set, it would be done here:
		// auto& transform = registry.get<Transform>(lightEntity);
		// transform.scale = scale; // Assuming Transform has a scale member
		// transform.orientation = orientation;

		// Add Light component
		registry.emplace<Light>(lightEntity, lightProperties);

		// Add SceneNode component and attach to parent
		registry.emplace<SceneNode>(lightEntity);
		if (registry.valid(parent)) {
			SceneNode::addChild(registry, parent, lightEntity);
		}
		// Else: it becomes a root object or handle error/warning if parent was expected

		return lightEntity;
	}

private:
	TextureManagerType& textureManager;
	MeshManagerType& meshManager;
};