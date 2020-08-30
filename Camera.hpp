#pragma once

#include <stdexcept>
#include <iostream>
#include <algorithm>

#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtc/quaternion.hpp>

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
	float mouselookSensitivity{ .5f };
	float movementSpeed{ .1f };
	glm::vec3 position;

	float pitch{ 0.f };
	float yaw{ 0.f };
	float roll{ 0.f };




	// Look at, say, {0.f, 0.f, 0.f}
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;

	Camera(glm::vec3 startingPosition, glm::vec3 lookingAt)
		: position{startingPosition}
		, forward{glm::normalize(lookingAt - startingPosition)}
		, up{ 0.f, 1.f, 0.f }
	{
		right = glm::normalize(glm::cross(up, forward));
		//up = glm::normalize(glm::cross(forward, right));
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

		// project walkVector onto xz plane so it doesn't climb or descend
		walkVector.y = 0.f;

		position = position + walkVector * movementSpeed;
	}

	void mouseLook(glm::vec2 mouseDelta, double deltaTime)
	{
		yaw += mouseDelta.x * mouselookSensitivity;
		if (yaw > 359.999f || yaw < -359.999f) {
			yaw = std::fmodf(yaw, 359.999f);
		}

		pitch += -mouseDelta.y * mouselookSensitivity;
		pitch = std::clamp(pitch, -80.f, 80.f);

		updateVectorsFromOrientationAngles();
	}

	void updateVectorsFromOrientationAngles()
	{
		forward = glm::normalize(glm::vec3{
			glm::sin(glm::radians(yaw)),
			glm::sin(glm::radians(pitch)),
			-glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw))
		});
		right = glm::cross(forward, up);

		printCameraDiagnostics();
	}

	void printCameraDiagnostics()
	{
		printf("====================\n");
		printf("Yaw: %f --- Pitch: %f\n", yaw, pitch);
		printVector3("Position: ", position);
		printVector3("Forward vector: ", forward);
		printVector3("Right vector: ", right);
		printVector3("Up vector: ", up);
		printf("====================\n");
	}

};

