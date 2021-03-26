#pragma once

//#include "$(ProjectDir)/Event/EventSystem.hpp"
#include "../../Event/EventSystem.hpp"
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct CharacterWalkEvent : public event::Event
{
	float Speed;
	glm::vec3 Direction;
};

struct CharacterLookEvent : public event::Event
{
	glm::vec2 LookDirection;
};


