#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "Transform.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"



class RenderSystem
{
public:
	RenderSystem(entt::registry& registry);
	void DrawScene(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, ShaderProgram& shaderProgram, int screenWidth, int screenHeight);

private:
	entt::registry& registry;
};

RenderSystem::RenderSystem(entt::registry& registry)
	: registry{ registry }
{
	glEnable(GL_CULL_FACE);
}

// for now we pass a ShaderProgram, but I'm not sure if it should be setup in the RenderSystem in a
// setup phase, from a mesh material that indicates what shader and textures should be used to render it
void RenderSystem::DrawScene(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, ShaderProgram& shaderProgram, int screenWidth, int screenHeight)
{
	// for now we simply draw all meshes
	shaderProgram.setMatrix4x4(viewMatrix, "view");
	shaderProgram.setMatrix4x4(projectionMatrix, "projection");

	// CHANGE: draw textured
	registry.view <Transform, std::shared_ptr<Mesh>, std::shared_ptr<Texture>>().each(
		[&](const Transform& transform, const std::shared_ptr<Mesh>& mesh_ptr, const std::shared_ptr<Texture>& texture_ptr) {


			glm::mat4 modelMatrix = transform.getModelMatrix();
			shaderProgram.setMatrix4x4(modelMatrix, "model");
			texture_ptr->bind();
			mesh_ptr->draw(shaderProgram);
			texture_ptr->unbind();
		}
	);
}
