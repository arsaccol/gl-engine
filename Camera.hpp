#pragma once

#include <stdexcept>
#include <iostream>

#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

#include "Helper.hpp"


class Camera
{
public:
	enum class WalkDirection
	{
		FORWARD,
		BACKWARD,
		STRAFE_LEFT,
		STRAFE_RIGHT,
		NO_WALK
	};


	// Start it at, say, {0, 0, 3.f}
	float movementSpeed{ .5f };
	glm::vec3 position;
	glm::vec3 eulerOrientation{ 0.f, 0.f, 0.f };

	// Look at, say, {0.f, 0.f, 0.f}
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;

	Camera(glm::vec3 startingPosition, glm::vec3 lookingAt)
		: position{startingPosition}
		, forward{glm::normalize(lookingAt)}
		, up{ 0.f, 1.f, 0.f }
	{
		right = glm::normalize(glm::cross(forward, up));
	}



	glm::mat4x4 getMatrix()
	{
		auto buga = right;
		return glm::lookAt(position, position + forward, up);
	}

	void walk(WalkDirection walkDirection)
	{
		//walkDirection = glm::normalize(walkDirection);

		glm::vec3 walkVector{ 0, 0, 0 };


		switch (walkDirection)
		{
		case Camera::WalkDirection::FORWARD:
			walkVector = forward;
			break;
		case Camera::WalkDirection::BACKWARD:
			walkVector = -forward;
			break;
		case Camera::WalkDirection::STRAFE_LEFT:
			walkVector = -right;
			break;
		case Camera::WalkDirection::STRAFE_RIGHT:
			walkVector = right;
			break;
		default:
			//throw std::runtime_error{ "Invalid walk direction for camera" };
			//walkVector = glm::zero <glm::vec3>();
			break;
		}

		position = position + walkVector * movementSpeed;
	}

	void mouseLook(float mouseDeltaX, float mouseDeltaY)
	{

	}

};

