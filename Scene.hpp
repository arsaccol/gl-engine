#pragma once

#include <cstdlib>
#include <entt/entt.hpp>
#include "./Transform.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Player.hpp"

#include "RenderSystem.hpp"
#include "Texture.hpp"

#include "ResourceManager.hpp"
#include "Prefabs/Factory.hpp"

#include "SceneNode.hpp"

class Scene
{
public:
	void setup();
	entt::registry& getRegistry();

	void update(double dt);
	void render(const int windowWidth, const int windowHeight);
private: 
	void setupCubeMesh();
	void loadResources();
	void setupSceneGraphRoot();
	void setupPlayer();

private:
	entt::registry registry;

	TextureManagerType& textureManager{ ResourceManager::textureManager };
	MeshManagerType& meshManager{ ResourceManager::meshManager };
	Factory factory;

	Player player{ registry };
	entt::entity playerEntity;
	std::shared_ptr<Transform> playerTransform;

	entt::entity rootNodeEntity;

	std::shared_ptr<Mesh> cubeMesh;
	std::unique_ptr<ShaderProgram> shaderProgram;
	std::unique_ptr<RenderSystem> renderer;
};


void Scene::setupSceneGraphRoot()
{
	rootNodeEntity = registry.create();
	registry.emplace<Transform>(rootNodeEntity);
	registry.emplace<SceneNode>(rootNodeEntity);
}


void Scene::setupPlayer()
{
	player.setup(rootNodeEntity);
	playerEntity = player.getEntity();
	playerTransform = registry.get<std::shared_ptr<Transform>>(playerEntity);
	registry.emplace<SceneNode>(playerEntity);
	SceneNode::addChild(registry, rootNodeEntity, playerEntity);

}

void Scene::setup()
{
	loadResources();
	renderer = std::make_unique<RenderSystem>(registry);

	setupSceneGraphRoot();

	setupPlayer();



	entt::entity humanID = factory.Human(registry, rootNodeEntity);
	Transform& humanTransform = registry.get<Transform>(humanID);

	humanTransform.rotateAroundAxis(glm::radians(45.f), { 0, 1, 0 });
	humanTransform.translateLocal(0, 0, -10);
	humanTransform.translate(5, 0, 0);

	for (int i = 0; i < 1; ++i)
	{
		factory.BlenderCube(registry, rootNodeEntity);
	}
}

entt::registry& Scene::getRegistry()
{
	return registry;
}

void Scene::loadResources()
{
	// TODO: create materials which contain the textures and shaders, associate materials with meshes

	// load meshes and textures
	meshManager.emplace("human", std::make_shared<Mesh>("models/better-human.obj"));
	textureManager.emplace("human", std::make_shared<Texture>("models/better-humanTexture.jpg"));
	meshManager.emplace("cube", std::make_shared<Mesh>("models/teapot-smooth.obj"));
	textureManager.emplace("cube", std::make_shared<Texture>("models/test-texture.jpg"));

	// load shaders
	shaderProgram = std::make_unique<ShaderProgram>(ShaderProgram::ShaderTargetFilenamePairs{
		{GL_VERTEX_SHADER, "shaders/phong-with-uniforms.vert"},
		{GL_FRAGMENT_SHADER, "shaders/phong-with-uniforms.frag"},
		});
	printGLError("After shader setup");
}


void Scene::setupCubeMesh()
{
	// this is wrong, we're creating one mesh for each cube
	// instead, we should 
	cubeMesh = std::make_shared<Mesh>(
		std::vector{
			// front
			Vertex{{-.5f, -.5f, .5f}, {0.f, 0.f},  {1.f, 0.f, 0.f}},
			Vertex{{.5f, -.5f, .5f}, {0.f, 0.f},  {0.f, 1.f, 0.f}},
			Vertex{{.5f, .5f, .5f}, {0.f, 0.f}, {0.f, 0.f, 1.f}},
			Vertex{{-.5f, .5f, .5f}, {0.f, 0.f}, {1.f, 1.f, 1.f}},
			// back 
			Vertex{{-.5f, -.5f, -.5f}, {0.f, 0.f}, {1.f, 0.f, 0.f}},
			Vertex{{.5f, -.5f, -.5f}, {0.f, 0.f}, {0.f, 1.f, 0.f}},
			Vertex{{.5f, .5f, -.5f}, {0.f, 0.f}, {0.f, 0.f, 1.f}},
			Vertex{{-.5f, .5f, -.5f}, {0.f, 0.f}, {1.f, 1.f, 1.f}}
		},
		// indices for index buffer
		std::vector<unsigned int>{
		// front
			0, 1, 2,
			2, 3, 0,
			// right
			1, 5, 6,
			6, 2, 1,
			// back
			7, 6, 5,
			5, 4, 7,
			// left
			4, 0, 3,
			3, 7, 4,
			// botttom
			4, 5, 1,
			1, 0, 4,
			// top
			3, 2, 6,
			6, 7, 3
		}
	);
}


void Scene::update(double dt)
{
	const float rotationSpeed = 1.f;
	registry.view<Transform>().each(
		[&](Transform& transform) {
			transform.rotateAroundAxis(rotationSpeed * dt, glm::vec3{ 0, 1, 0 });
		}
	);
}

void Scene::render(const int windowWidth, const int windowHeight)
{
	glm::vec3 position = playerTransform->position;
	glm::vec3 forward = playerTransform->forward();
	glm::vec3 up = playerTransform->up();

	glm::mat4 view = glm::lookAt(position, position + forward, up);

	glm::mat4 projection = glm::perspective<float>(glm::radians(60.f), (float)windowWidth / (float)windowHeight, 1.f, 100.f);

	//renderer->DrawScene(position, view, projection, *shaderProgram, windowWidth, windowHeight);
	renderer->DrawSceneGraph(position, view, projection, *shaderProgram, windowWidth, windowHeight, rootNodeEntity);

	player.Debug();
}