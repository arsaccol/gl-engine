#pragma once

#include <functional>
#include <memory>
#include <cstdio>

#include <glm/vec3.hpp>

#include "./Input/InputSystem.hpp"
#include "./Event/Observer.hpp"
#include "./Input/InputEvents/CharacterInputEvent.hpp"

#include "./Camera.hpp"

#include "Helper.hpp"


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

private:
	void registerLookHandler();
	void registerWalkHandler();

private:
	glm::vec3 position;
	glm::quat rotation;
public:
	Camera camera{ glm::vec3{0, 0, 3}, glm::vec3{0, 0, -1} };


};

Player::Player()
{
	registerLookHandler();
	registerWalkHandler();
}

void Player::registerLookHandler()
{
	auto& eventDispatcher = event::EventDispatcher::Instance();

	auto cameraLookHandler = [&](const event::Event& e) {
		const auto& lookEvent = dynamic_cast<const CharacterLookEvent&>(e);
		printVector2("lookEvent.LookDirection", lookEvent.LookDirection);

		camera.mouseLook(lookEvent.LookDirection);
	};

	eventDispatcher.On<CharacterLookEvent>(cameraLookHandler);
}

void Player::registerWalkHandler()
{
	auto& eventDispatcher = event::EventDispatcher::Instance();

	auto walkHandler = [&](const event::Event& e) {
		const auto& walkEvent = dynamic_cast<const CharacterWalkEvent&>(e);
		printf("walkEvent.DirectionFlags %04x\n", walkEvent.DirectionFlags);

		auto& eventWalkDirection = walkEvent.DirectionFlags;


		// this is stupid and defeats the purpose of having flags, but
		// for now we're doing it to simply remove the camera reference from the Input class;
		// afterwards we can use a proper vector or something to dictate walk movement with
		// diagonal directions and all that stuff

		auto legacyWalkDirection = Camera::WalkDirection::NO_WALK;

		if (eventWalkDirection & WalkDirections::FORWARD)
			legacyWalkDirection = Camera::WalkDirection::FORWARD;
		else if (eventWalkDirection & WalkDirections::BACKWARD)
			legacyWalkDirection = Camera::WalkDirection::BACKWARD;
		else if (eventWalkDirection & WalkDirections::STRAFE_LEFT)
			legacyWalkDirection = Camera::WalkDirection::STRAFE_LEFT;
		else if (eventWalkDirection & WalkDirections::STRAFE_RIGHT)
			legacyWalkDirection = Camera::WalkDirection::STRAFE_RIGHT;

		camera.walk(legacyWalkDirection);
	};

	eventDispatcher.On<CharacterWalkEvent>(walkHandler);
}
