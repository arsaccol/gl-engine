#pragma once

#include <GLFW/glfw3.h>
#include "../Event/EventSystem.hpp"
#include "../Window.hpp"
#include <glm/glm.hpp>

#include "./InputEvents/CharacterInputEvent.hpp"
#include "../Camera.hpp"

#include "../Helper.hpp"


class Input : public event::Observable
{
public:
	Input(Window& window, Camera& camera);
	void processInput(float deltaTime);

private:
	void updateInput();
	void mouseMotionInput(float deltaTime);
	void initMouseInput();

private:
	GLFWwindow* window;
	glm::vec2 lastMousePosition;
	glm::vec2 mouseDelta;
	Camera& camera;
};

// ============ method implementations ============

Input::Input(Window& window, Camera& camera)
	: window{ window.getAPIWindowPtr() }
	, camera{ camera }
{
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(this->window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	else {
		throw std::runtime_error{ "Raw mouse input not supported" };
	}


	glfwSetInputMode(this->window, GLFW_CURSOR_DISABLED, GLFW_TRUE);
}

void Input::initMouseInput()
{
	double xpos, ypos;
	glfwGetCursorPos(this->window, &xpos, &ypos);
	glm::vec2 currentMousePosition{ static_cast<float>(xpos), static_cast<float>(ypos) };
	lastMousePosition = currentMousePosition;
}


void Input::updateInput()
{
}

// handleMouseInput
void Input::mouseMotionInput(float deltaTime)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	glm::vec2 currentMousePosition{ static_cast<float>(xpos), static_cast<float>(ypos) };
	mouseDelta = currentMousePosition - lastMousePosition;
	// printVector2(mouseDelta) etc etc 
	lastMousePosition = currentMousePosition;

	// This is for the future pretty much, right now we ape the old window input code
	//*** normally: 
	camera.mouseLook(mouseDelta, deltaTime);
	//*** actually meant to be: 
	//***CharacterLookEvent event;
	//***event.LookDirection = mouseDelta;
	//***SendEvent<CharacterLookEvent>(event);

}

// almost the same as before; call this function from outside I guess
// just one difference is we now have the deltaTime thing... goddamn it
void Input::processInput(float deltaTime)
{
	mouseMotionInput(deltaTime);

	Camera::WalkDirection walkDirection{ Camera::WalkDirection::NO_WALK };

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		walkDirection = Camera::WalkDirection::FORWARD;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		walkDirection = Camera::WalkDirection::BACKWARD;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		walkDirection = Camera::WalkDirection::STRAFE_LEFT;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		walkDirection = Camera::WalkDirection::STRAFE_RIGHT;
	camera.walk(walkDirection);
};
