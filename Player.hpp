#pragma once

#include <functional>
#include <memory>
#include <cstdio>

#include <glm/vec3.hpp>

#include "./Input/InputSystem.hpp"
#include "./Event/Observer.hpp"
#include "./Input/InputEvents/CharacterInputEvent.hpp"

#include "./Camera.hpp"
#include "./Transform.hpp"

#include "Helper.hpp"

#include <imgui.h>
#include <entt/entt.hpp>


template <typename EventSubType>
std::function<void(const EventSubType&)> createEventHandler(const event::EventDispatcher::EventHandler& eh)
{
}



// Game-logic related player stuff, like input event handlers
// and actual player related data, idk fam

// TODO: we move camera rotation logic out of the camera and into the player

#include <imgui.h>

class Player
{
public:
	struct PlayerParams
	{
		float mouseLookSensitivity{ 5 };
		float movementSpeed{ .1f };
		float pitch{ 0 };
		float yaw{ 0 };
	};


public:
	// upon construction, creates a player entity in the registry,
	// emplacing a transform, some PlayerParams, and a Camera (not done yet)
	// Camera is dependent upon player's Transform, and only carries "config" info such as FOV and whatnot
	Player(entt::registry& registry);

	void Debug();
private:
	entt::entity makePlayerEntity(const glm::vec3& position, const glm::vec3& rotationEulerAngles);
	void registerLookHandler();
	void registerWalkHandler();
	void registerResetHandlers();

private:
	entt::registry& registry;

	entt::entity playerEntity;
	std::shared_ptr<Transform> transform;
	std::shared_ptr<PlayerParams> playerParams;

	//float mouselookSensitivity{ 5.f };
	//float movementSpeed{ .1f };

	glm::vec3 walkVector;
public:
	//Camera camera{ glm::vec3{0, 0, 3}, glm::vec3{0, 0, -1} };
	entt::entity getEntity() 
	{
		return playerEntity;
	}

};

Player::Player(entt::registry& registry)
	: registry{ registry }
{
	makePlayerEntity(glm::vec3{ 0, 1.65, 3 }, glm::vec3{ 0, glm::radians(0.f), 0 });

	registerLookHandler();
	registerWalkHandler();
	registerResetHandlers();
}

entt::entity Player::makePlayerEntity(const glm::vec3& position, const glm::vec3& rotationEulerAngles)
{
	playerEntity = registry.create();
	playerParams = registry.emplace<std::shared_ptr<PlayerParams>>(playerEntity, 
		std::make_shared<PlayerParams>());

	transform = registry.emplace<std::shared_ptr<Transform>>(playerEntity, 
		std::make_shared<Transform>(position, rotationEulerAngles));

	return playerEntity;
}

void Player::Debug()
{
	using namespace ImGui;
	BeginChild("Player");
		transform->Debug();

		Begin("Walk vector");
			Text("x: %.6f y: %.6f z: %.6f", walkVector.x, walkVector.y, walkVector.z);
			Text("Magnitude: %.6f", glm::length(walkVector));
		End();
	EndChild();
}

void Player::registerLookHandler()
{
	auto& eventDispatcher = event::EventDispatcher::Instance();

	auto cameraLookHandler = [&](const event::Event& e) {
		const auto& lookEvent = dynamic_cast<const CharacterLookEvent&>(e);

		transform->rotateAroundAxis(lookEvent.LookDirection.x * -.001f * playerParams->mouseLookSensitivity , { 0, 1, 0 });
		transform->rotateAroundAxis(lookEvent.LookDirection.y * -.001f * playerParams->mouseLookSensitivity , transform->right());

		const glm::vec3 radianEulers = glm::eulerAngles(transform->orientation);
		glm::vec3 degreeEulers = glm::degrees(radianEulers);
	};

	eventDispatcher.On<CharacterLookEvent>(cameraLookHandler);
}

void Player::registerWalkHandler()
{
	auto& eventDispatcher = event::EventDispatcher::Instance();

	auto walkHandler = [&](const event::Event& e) {

		const auto& walkEvent = dynamic_cast<const CharacterWalkEvent&>(e);
		//printf("walkEvent.DirectionFlags %04x\n", walkEvent.DirectionFlags);

		auto& eventWalkDirection = walkEvent.DirectionFlags;

		// this is stupid and defeats the purpose of having flags, but
		// for now we're doing it to simply remove the camera reference from the Input class;
		// afterwards we can use a proper vector or something to dictate walk movement with
		// diagonal directions and all that stuff

		auto legacyWalkDirection = Camera::WalkDirection::NO_WALK;

		walkVector = { 0, 0, 0 };

		if (eventWalkDirection & WalkDirections::FORWARD) {
			//legacyWalkDirection = Camera::WalkDirection::FORWARD;
			walkVector = walkVector + transform->forward();
		}
		if (eventWalkDirection & WalkDirections::BACKWARD) {
			//legacyWalkDirection = Camera::WalkDirection::BACKWARD;
			walkVector = walkVector + -transform->forward();
		}
		if (eventWalkDirection & WalkDirections::STRAFE_LEFT) {
			//legacyWalkDirection = Camera::WalkDirection::STRAFE_LEFT;
			walkVector = walkVector + -transform->right();
		}
		if (eventWalkDirection & WalkDirections::STRAFE_RIGHT) {
			//legacyWalkDirection = Camera::WalkDirection::STRAFE_RIGHT;
			walkVector = walkVector + transform->right();
		}

		// normalize walk vector, but don't try to normalize zero vector
		if (glm::length(walkVector) > FLT_EPSILON)
			walkVector = glm::normalize(walkVector);

		//transform.position += walkVector * movementSpeed;
		walkVector *= walkEvent.dt * walkEvent.Speed;
		float x = walkVector.x; float z = walkVector.z;
		transform->translate(glm::vec3{ x, 0, z } * playerParams->movementSpeed);

		//camera.walk(legacyWalkDirection);
	};

	eventDispatcher.On<CharacterWalkEvent>(walkHandler);
}

void Player::registerResetHandlers()
{
	auto& eventDispatcher = event::EventDispatcher::Instance();

	auto resetPositionHandler = [&](const event::Event& e) {
		transform->position = { 0, 0, 3.f };
	};

	eventDispatcher.On<CharacterResetPositionEvent>(resetPositionHandler);
}
