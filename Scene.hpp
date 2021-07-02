#pragma once

#include <entt/entt.hpp>
#include "./Transform.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Player.hpp"

class Scene
{
public:
	void setup();

	void update();
	void render(const int windowWidth, const int windowHeight);
private: 
	void setupCube(const glm::vec3& position);

private:
	entt::registry registry;

	Player player;
	std::unique_ptr<ShaderProgram> shaderProgram;
};

void Scene::setup()
{
	shaderProgram = std::make_unique<ShaderProgram>(ShaderProgram::ShaderTargetFilenamePairs{
		{GL_VERTEX_SHADER, "vertex_shader.vert"},
		{GL_FRAGMENT_SHADER, "fragment_shader.frag"},
	});

	printGLError("After shader setup");

	// TODO: create resource manager classes, for meshes, textures, audio, shaders, etc.
	// good example would be like in the SFML book: ResourceManager<Texture> resMgr, then load stuff from files,
	// and have registry entities contain references to those
	// As it is right now, the registry recreates the same meshes for each entity, which is definitely 
	// not desirable lol

	auto humanEntity = registry.create();
	registry.emplace<Mesh>(humanEntity, "models/better-human.obj");
	registry.emplace<Transform>(humanEntity);

	setupCube(glm::vec3{ 1.5, 0.5, 0 });
}

void Scene::setupCube(const glm::vec3& position)
{
	// this is wrong, we're creating one mesh for each cube
	// instead, we should 
	auto cubeEntity = registry.create();
	registry.emplace<Mesh>(cubeEntity,
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

	registry.emplace<Transform>(cubeEntity, position);
}


void Scene::update()
{
}

void Scene::render(const int windowWidth, const int windowHeight)
{
	glm::vec3 position = player.transform.position;
	glm::vec3 forward = player.transform.forward();
	glm::vec3 up = player.transform.up();

	glm::mat4 view = glm::lookAt(position, position + forward, up);

	


	glm::mat4 projection = glm::perspective<float>(glm::radians(60.f), (float)windowWidth / (float)windowHeight, 1.f, 100.f);
	glm::mat4 view_projection =  projection * view;

	auto renderView = registry.view<Transform, Mesh>();
	for (auto& renderEntity : renderView)
	{
		auto& mesh = renderView.get<Mesh>(renderEntity);
		auto& transform = renderView.get<Transform>(renderEntity);

		auto modelMatrix = transform.getModelMatrix();

		glm::mat4 mvp = view_projection * modelMatrix;
		shaderProgram->setMatrix4x4(mvp, "MVP");
		mesh.draw(*shaderProgram);
	}

	player.Debug();
}