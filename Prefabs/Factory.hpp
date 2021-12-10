#pragma once 

#include <entt/entt.hpp>
#include <memory>
#include "../Mesh.hpp"
#include "../Texture.hpp"
#include "../Transform.hpp"
#include "../ResourceManager.hpp"
#include "../SceneNode.hpp"

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
				static_cast<float>(rand() % 100 - 50),
				static_cast<float>(rand() % 100 - 50),
				static_cast<float>(rand() % 100 - 50),
			}
		);

		registry.emplace<std::shared_ptr<Mesh>>(cubeEntity, meshManager["cube"]);
		registry.emplace<std::shared_ptr<Texture>>(cubeEntity, textureManager["cube"]);

		return cubeEntity;
	}

private:
	TextureManagerType& textureManager;
	MeshManagerType& meshManager;
};