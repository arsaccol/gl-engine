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


template <typename EventSubType>
std::function<void(const EventSubType&)> createEventHandler(const event::EventDispatcher::EventHandler& eh)
{
}



// Game-logic related player stuff, like input event handlers
// and actual player related data, idk fam

// TODO: we move camera rotation logic out of the camera and into the player

class Player
{
public:
	Player();

	void Debug();

private:
	void registerLookHandler();
	void registerWalkHandler();

private:
	// these should go in the transform and not here, but keep them commented out
	//glm::vec3 position;
	//glm::quat rotation;

	float mouselookSensitivity{ 5.f };
	float movementSpeed{ .1f };

	float pitch = 0.f;
	float yaw = 0.f;


public:
	Camera camera{ glm::vec3{0, 0, 3}, glm::vec3{0, 0, -1} };
	Transform transform{ {0, 0, 3}, {0, glm::radians(0.f), 0} };


};

Player::Player()
{
	registerLookHandler();
	registerWalkHandler();
}

void Player::Debug()
{
	using namespace ImGui;
	BeginChild("Player");
		transform.Debug();
	EndChild();
}

void Player::registerLookHandler()
{
	auto& eventDispatcher = event::EventDispatcher::Instance();

	auto cameraLookHandler = [&](const event::Event& e) {
		const auto& lookEvent = dynamic_cast<const CharacterLookEvent&>(e);
		//printVector2("lookEvent.LookDirection", lookEvent.LookDirection);

		camera.mouseLook(lookEvent.LookDirection);

		transform.rotateAroundAxis(lookEvent.LookDirection.x * -.001f * mouselookSensitivity , { 0, 1, 0 });
		transform.rotateAroundAxis(lookEvent.LookDirection.y * -.001f * mouselookSensitivity , transform.right());


		const glm::vec3 radianEulers = glm::eulerAngles(transform.orientation);
		glm::vec3 degreeEulers = glm::degrees(radianEulers);

		//printVector3("Right vector coordinates", transform.right());


		printVector3("Transform Eulers (radians)", radianEulers * glm::pi<float>());
		printVector3("Transform Eulers (degrees):", degreeEulers);

		// we're trying to implement correctly this now
		// hopefully
		//transform.mouseRotation(lookEvent.LookDirection, mouselookSensitivity);
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

		glm::vec3 walkVector{ 0, 0, 0 };

		if (eventWalkDirection & WalkDirections::FORWARD) {
			//legacyWalkDirection = Camera::WalkDirection::FORWARD;
			walkVector = glm::normalize(walkVector + transform.forward());
		}
		if (eventWalkDirection & WalkDirections::BACKWARD) {
			//legacyWalkDirection = Camera::WalkDirection::BACKWARD;
			walkVector = glm::normalize(walkVector + -transform.forward());
		}
		if (eventWalkDirection & WalkDirections::STRAFE_LEFT) {
			//legacyWalkDirection = Camera::WalkDirection::STRAFE_LEFT;
			walkVector = glm::normalize(walkVector + -transform.right());
		}
		if (eventWalkDirection & WalkDirections::STRAFE_RIGHT) {
			//legacyWalkDirection = Camera::WalkDirection::STRAFE_RIGHT;
			walkVector = glm::normalize(walkVector + transform.right());
		}


		//transform.position
		transform.position += walkVector * movementSpeed;

		//camera.walk(legacyWalkDirection);
	};

	eventDispatcher.On<CharacterWalkEvent>(walkHandler);
}
