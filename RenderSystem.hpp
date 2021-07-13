#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "Transform.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"



class RenderSystem
{
public:
	RenderSystem(entt::registry& registry);
	void DrawScene(const glm::mat4& viewProjectionMatrix, ShaderProgram& shaderProgram, int screenWidth, int screenHeight);

private:
	entt::registry& registry;
};

RenderSystem::RenderSystem(entt::registry& registry)
	: registry{ registry }
{
}

// for now we pass a ShaderProgram, but I'm not sure if it should be setup in the RenderSystem in a
// setup phase, from a mesh material that indicates what shader and textures should be used to render it
void RenderSystem::DrawScene(const glm::mat4& viewProjectionMatrix, ShaderProgram& shaderProgram, int screenWidth, int screenHeight)
{
	// for now we simply draw all meshes
	registry.view <Transform, std::shared_ptr<Mesh>>().each(
		[&](const Transform& transform, const std::shared_ptr<Mesh>& mesh_ptr) {
			glm::mat4 modelMatrix = transform.getModelMatrix();
			glm::mat4 mvp = viewProjectionMatrix * modelMatrix;
			shaderProgram.setMatrix4x4(mvp, "MVP");
			mesh_ptr->draw(shaderProgram);
		}
	);
}
