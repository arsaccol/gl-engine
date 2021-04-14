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
class Player
{
public:
	Player();

private:
	glm::vec3 position;
	glm::quat rotation;

public:
	Camera camera{ glm::vec3{0, 0, 3}, glm::vec3{0, 0, -1} };

};

Player::Player()
{
	auto& eventDispatcher = event::EventDispatcher::Instance();

	auto cameraLookHandler = [&](const event::Event& e) {
		const auto& lookEvent = dynamic_cast<const CharacterLookEvent&>(e);
		printVector2("lookEvent.LookDirection", lookEvent.LookDirection);
	};

	eventDispatcher.On<CharacterLookEvent>(cameraLookHandler);

}
