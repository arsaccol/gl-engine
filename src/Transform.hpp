#pragma once

#include <cmath>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp> // do we need this?

#include "Helper.hpp"

#include "imgui.h"

class Transform
{
public:

	glm::vec3 position;
	// start as unit quaternion with no 
	glm::quat orientation = glm::quat{ 1, 0, 0, 0 };

	// Transform implementation idea:
	// to obtain forward vector, take the vector {0, 0, 1} (maybe flipped because of right-hand system?)...
	// and rotate it by our internal quaternion. same goes for up vector, take the 

	Transform(
		glm::vec3 initialPosition = { 0.f,0.f,0.f },
		glm::vec3 rotationEulerAngles = { 0.f,0.f,0.f }
	)
		: position{initialPosition}
		, orientation{rotationEulerAngles}
	{
	}

	void Debug()
	{
		using namespace ImGui;

		BeginChild("Transform");
			Begin("Position");
				Text("x: %.6f y: %.6f z: %.6f", position.x, position.y, position.z);
			End();

			Begin("Orientation");
				auto eulers = glm::degrees(glm::eulerAngles(orientation));
				Text("Pitch: %3.6f Yaw: %3.5f Roll: %3.5f", eulers.x, eulers.y, eulers.z);
			End();
		EndChild();
	}

	glm::vec3 forward() const
	{
		//return orientation * glm::vec3{ 0.f, 0.f, 1.f };
		return glm::rotate(orientation, glm::vec3{ 0, 0, -1 });
		return { 0, 0, 1 };
	}

	glm::vec3 up() const
	{
		//return orientation * glm::vec3{ 0.f, 1.f, 0.f };
		return glm::rotate(orientation, glm::vec3{ 0, 1, 0 });
		return { 0, 1, 0 };
	}

	glm::vec3 right() const
	{
		//return orientation * glm::vec3{ 1.f, 0.f, 0.f };
		return glm::rotate(orientation, glm::vec3{ 1, 0, 0 });
		//return { 1, 0, 0 };
	}

	glm::vec3 translateLocal(const glm::vec3 translation)
	{
		position += glm::rotate(orientation, translation);
		return position;
	}

	glm::vec3 translateLocal(float x, float y, float z)
	{
		glm::vec3 absoluteTranslation = glm::vec3{ x, y, z };
		position += glm::rotate(orientation, absoluteTranslation);

		return position;
	}


	glm::vec3 translate(const glm::vec3& translation)
	{
		position += translation;
		return position;
	}

	glm::vec3 translate(float x, float y, float z)
	{
		position.x += x;
		position.y += y;
		position.z += z;

		return position;
	}


	glm::quat rotateAroundAxis(float angleRadians, glm::vec3 axis)
	{
		orientation = glm::angleAxis(angleRadians, axis) * orientation;
		return orientation;
	}

	// this is wrong (I think): don't use angle axis
	// according to answers at: https://stackoverflow.com/questions/28776788/quaternion-reaching-gimbal-lock
	//glm::quat rotateAngleAxis(const float angleRadians, const glm::vec3& axis)
	//{
	//	rotation = rotation * glm::angleAxis(angleRadians, axis);
	//	return rotation;
	//}




	const glm::mat4 getModelMatrix() const
	{
		//auto translation = glm::translate(glm::mat4{}, position);
		//auto rotation = glm::rotate(translation);
		glm::mat4 translation = glm::translate(glm::identity<glm::mat4>(), position);
		glm::mat4 rotation = glm::toMat4(orientation);

		//return rotation * translation;
		return translation * rotation;
	}

};
