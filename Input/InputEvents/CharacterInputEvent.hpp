#pragma once

//#include "$(ProjectDir)/Event/EventSystem.hpp"
#include "../../Event/EventSystem.hpp"
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

enum WalkDirections
{
	FORWARD = 1,
	BACKWARD = 2,
	STRAFE_LEFT = 4,
	STRAFE_RIGHT = 8
};

struct CharacterWalkEvent : public event::Event
{
	float Speed;

	int DirectionFlags;
};

struct CharacterLookEvent : public event::Event
{
	glm::vec2 LookDirection;
};


