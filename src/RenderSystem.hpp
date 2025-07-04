#pragma once

#include <stack>
#include <tuple>

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "Transform.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "SceneNode.hpp"
#include "Light.hpp" // Added for Light class



class RenderSystem
{
public:
	RenderSystem(entt::registry& registry);
	void DrawSceneGraph(const glm::vec3& cameraPosition, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, ShaderProgram& shaderProgram, int screenWidth, int screenHeight, entt::entity sceneGraphRoot);
	void DrawScene(const glm::vec3& cameraPosition, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, ShaderProgram& shaderProgram, int screenWidth, int screenHeight);

private:
	using EntityWithMatrix = std::tuple<entt::entity, glm::mat4>;
	void DrawMesh(Mesh& mesh, Texture& texture, const glm::vec3& cameraPosition, const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, ShaderProgram& shaderProgram);
	std::vector<EntityWithMatrix> getDrawableEntitiesInOrder(entt::entity root);
	

	entt::registry& registry;
};

RenderSystem::RenderSystem(entt::registry& registry)
	: registry{ registry }
{
	glEnable(GL_CULL_FACE);
}

void RenderSystem::DrawMesh(Mesh& mesh, Texture& texture, const glm::vec3& cameraPosition, const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, ShaderProgram& shaderProgram)
{
	shaderProgram.setVec3(cameraPosition, "cameraPosition");
	shaderProgram.setMatrix4x4(viewMatrix, "view");
	shaderProgram.setMatrix4x4(projectionMatrix, "projection");
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));

	shaderProgram.setMatrix3x3(normalMatrix, "normalMatrix");
	shaderProgram.setMatrix4x4(modelMatrix, "model");
	texture.bind();
	mesh.draw(shaderProgram);
	texture.unbind();
}

void RenderSystem::DrawSceneGraph(const glm::vec3& cameraPosition, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, ShaderProgram& shaderProgram, int screenWidth, int screenHeight, entt::entity sceneGraphRoot)
{
	struct ShaderLightData {
		glm::vec3 position;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};
	std::vector<ShaderLightData> lightsForShader;
	auto lightView = registry.view<::Light, Transform, SceneNode>(); // Added SceneNode to the view
	for (auto entity : lightView) {
		const auto& lightComponent = lightView.get<::Light>(entity);
		const auto& transformComponent = lightView.get<Transform>(entity);
		const auto& lightSceneNode = lightView.get<SceneNode>(entity);

		glm::mat4 lightWorldMatrix = transformComponent.getModelMatrix();
		entt::entity parentEntity = lightSceneNode.parent;

		// Traverse up the scene graph to calculate world position
		while (registry.valid(parentEntity) && parentEntity != sceneGraphRoot) {
			if (!registry.all_of<Transform, SceneNode>(parentEntity)) {
				// Log warning or handle error: parent is missing required components
				break;
			}
			const auto& parentTransform = registry.get<Transform>(parentEntity);
			lightWorldMatrix = parentTransform.getModelMatrix() * lightWorldMatrix;

			const auto& parentSceneNode = registry.get<SceneNode>(parentEntity);
			parentEntity = parentSceneNode.parent;
		}

		glm::vec3 worldLightPosition = glm::vec3(lightWorldMatrix[3]);

		ShaderLightData sld;
		sld.position = worldLightPosition;
		sld.ambient = lightComponent.ambient;
		sld.diffuse = lightComponent.diffuse;
		sld.specular = lightComponent.specular;

		lightsForShader.push_back(sld);
	}
	shaderProgram.setLights(lightsForShader, "lights");

	auto drawablesView = registry.view<SceneNode, Transform, std::shared_ptr<Mesh>, std::shared_ptr<Texture>>();
	drawablesView.each(
		[&](SceneNode& sceneNode, Transform& transform, std::shared_ptr<Mesh>& mesh_ptr, std::shared_ptr<Texture>& texture) {

			glm::mat4 worldMatrix{ transform.getModelMatrix() };

			// move upwards in scene tree to build a global world matrix for this mesh
			auto currentDrawable = std::make_tuple(sceneNode, transform);
			while (std::get<0>(currentDrawable).parent != sceneGraphRoot)
			{
				// ERROR: we have an infinite loop here
				entt::entity parentEntity = std::get<0>(currentDrawable).parent;
				SceneNode& parentSceneNode = drawablesView.get<SceneNode>(parentEntity);
				Transform& parentTransform = drawablesView.get<Transform>(parentEntity);

				worldMatrix = parentTransform.getModelMatrix() * worldMatrix;

				currentDrawable = std::make_tuple(parentSceneNode, parentTransform);
			}

			// finally, draw mesh with the computed world matrix
			DrawMesh(*mesh_ptr, *texture, cameraPosition, worldMatrix, viewMatrix, projectionMatrix, shaderProgram);

		});
}


std::vector<RenderSystem::EntityWithMatrix> RenderSystem::getDrawableEntitiesInOrder(entt::entity root)
{
	return {};
}




// for now we pass a ShaderProgram, but I'm not sure if it should be setup in the RenderSystem in a
// setup phase, from a mesh material that indicates what shader and textures should be used to render it
void RenderSystem::DrawScene(const glm::vec3& cameraPosition, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, ShaderProgram& shaderProgram, int screenWidth, int screenHeight)
{
	struct ShaderLightData {
		glm::vec3 position;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};
	std::vector<ShaderLightData> lightsForShader;
	auto lightView = registry.view<::Light, Transform>();
	for (auto entity : lightView) {
		const auto& lightComponent = lightView.get<::Light>(entity);
		const auto& transformComponent = lightView.get<Transform>(entity);

		ShaderLightData sld;
		sld.position = transformComponent.position;
		sld.ambient = lightComponent.ambient;
		sld.diffuse = lightComponent.diffuse;
		sld.specular = lightComponent.specular;

		lightsForShader.push_back(sld);
	}
	shaderProgram.setLights(lightsForShader, "lights");

	// for now we simply draw all meshes
	shaderProgram.setVec3(cameraPosition, "cameraPosition");
	shaderProgram.setMatrix4x4(viewMatrix, "view");
	shaderProgram.setMatrix4x4(projectionMatrix, "projection");
	//shaderProgram.set

	// CHANGE: draw textured
	registry.view <Transform, std::shared_ptr<Mesh>, std::shared_ptr<Texture>>().each(
		[&](const Transform& transform, const std::shared_ptr<Mesh>& mesh_ptr, const std::shared_ptr<Texture>& texture_ptr) {

			glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(transform.getModelMatrix())));

			shaderProgram.setMatrix3x3(normalMatrix, "normalMatrix");

			glm::mat4 modelMatrix = transform.getModelMatrix();
			shaderProgram.setMatrix4x4(modelMatrix, "model");
			texture_ptr->bind();
			mesh_ptr->draw(shaderProgram);
			texture_ptr->unbind();
		}
	);
}
