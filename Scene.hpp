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

class Scene
{
public:
	void setup();
	entt::registry& getRegistry();

	void update();
	void render(const int windowWidth, const int windowHeight);
private: 
	void setupCubeMesh();
	void loadResources();

private:
	entt::registry registry;

	TextureManagerType& textureManager{ ResourceManager::textureManager };
	MeshManagerType& meshManager{ ResourceManager::meshManager };
	Factory factory;

	Player player{ registry };
	entt::entity playerEntity;
	std::shared_ptr<Transform> playerTransform;

	std::shared_ptr<Mesh> cubeMesh;
	std::unique_ptr<ShaderProgram> shaderProgram;
	std::unique_ptr<RenderSystem> renderer;
};

void Scene::setup()
{
	playerEntity = player.getEntity();
	playerTransform = registry.get<std::shared_ptr<Transform>>(playerEntity);

	shaderProgram = std::make_unique<ShaderProgram>(ShaderProgram::ShaderTargetFilenamePairs{
		{GL_VERTEX_SHADER, "shaders/diffuse_lighting_shader.vert"},
		{GL_FRAGMENT_SHADER, "shaders/diffuse_lighting_shader.frag"},
		});

	renderer = std::make_unique<RenderSystem>(registry);

	printGLError("After shader setup");

	loadResources();

	auto humanID = factory.Human(registry);
	Transform& humanTransform = registry.get<Transform>(humanID);
	humanTransform.rotateAroundAxis(glm::radians(45.f), { 0, 1, 0 });
	humanTransform.translateLocal(0, 0, -10);
	humanTransform.translate(5, 0, 0);


	//setupCubeMesh();

	for (int i = 0; i < 500; ++i)
	{
		factory.BlenderCube(registry);
	}
}

entt::registry& Scene::getRegistry()
{
	return registry;
}

void Scene::loadResources()
{
	meshManager.emplace("human", std::make_shared<Mesh>("models/better-human.obj"));
	textureManager.emplace("human", std::make_shared<Texture>("models/better-humanTexture.jpg"));
	meshManager.emplace("cube", std::make_shared<Mesh>("models/blender-cube.obj"));
	textureManager.emplace("cube", std::make_shared<Texture>("models/test-texture.jpg"));
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


void Scene::update()
{
}

void Scene::render(const int windowWidth, const int windowHeight)
{
	glm::vec3 position = playerTransform->position;
	glm::vec3 forward = playerTransform->forward();
	glm::vec3 up = playerTransform->up();

	glm::mat4 view = glm::lookAt(position, position + forward, up);

	glm::mat4 projection = glm::perspective<float>(glm::radians(60.f), (float)windowWidth / (float)windowHeight, 1.f, 100.f);
	glm::mat4 view_projection =  projection * view;

	renderer->DrawScene(view_projection, *shaderProgram, windowWidth, windowHeight);

	player.Debug();
}