#pragma once 

#include <entt/entt.hpp>
#include <memory>
#include "../Mesh.hpp"
#include "../Texture.hpp"
#include "../Transform.hpp"
#include "../ResourceManager.hpp"

struct Factory
{
	// Factory functions rely on resources having already been loaded
	Factory()
		: textureManager{ ResourceManager::textureManager },
		meshManager{ResourceManager::meshManager}
	{

	}


	auto Human(entt::registry& registry)
	{
		//textureManager.emplace("human", std::make_shared<Texture>("models/better-humanTexture.jpg"));
		//meshManager.emplace("human", std::make_shared<Mesh>("models/better-human.obj"));

		auto humanEntity = registry.create();

		registry.emplace<std::shared_ptr<Mesh>>(humanEntity, meshManager["human"]);
		registry.emplace<Transform>(humanEntity);
		registry.emplace<std::shared_ptr<Texture>>(humanEntity, textureManager["human"]);

		return humanEntity;
	}

	auto BlenderCube(entt::registry& registry)
	{
		//textureManager.emplace("blender-cube", std::make_shared<Texture>("models/test-texture.jpg"));
		//meshManager.emplace("blender-cube", std::make_shared<Mesh>("models/blender-cube.obj"));

		auto cubeEntity = registry.create();
		registry.emplace<Transform>(cubeEntity, 
			glm::vec3{
				static_cast<float>(rand() % 100 - 50),
				static_cast<float>(rand() % 100 - 50),
				static_cast<float>(rand() % 100 - 50),
			}
		);

		registry.emplace<std::shared_ptr<Mesh>>(cubeEntity, meshManager["cube"]);
		registry.emplace<std::shared_ptr<Texture>>(cubeEntity, textureManager["cube"]);
	}

private:
	TextureManagerType& textureManager;
	MeshManagerType& meshManager;
};