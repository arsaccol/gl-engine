#pragma once

#include <GLFW/glfw3.h>
#include "../Event/EventSystem.hpp"
#include "../Window.hpp"
#include <glm/glm.hpp>

#include "./InputEvents/CharacterInputEvent.hpp"
#include "../Camera.hpp"

#include "../Helper.hpp"
#include <memory>


// requires calling Input::setupInput() before using
class Input : public event::Observable
{
public:
	void setup(Window& window);
	void processInput(float deltaTime);

private:
	void updateInput();
	void mouseMotionInput(float deltaTime);
	void initMouseInput();

public:
	static void mouseCursorCallback(GLFWwindow* window, double xpos, double ypos);

private:
	Window* window;
	static glm::vec2 lastMousePosition;
	static glm::vec2 mouseDelta;
};

// static member variable definitions
glm::vec2 Input::lastMousePosition;
glm::vec2 Input::mouseDelta;

// ============ method implementations ============


void Input::setup(Window& window)
{
	this->window = &window;


	glfwSetInputMode(this->window->getAPIWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(this->window->getAPIWindowPtr(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	else {
		throw std::runtime_error{ "Raw mouse input not supported" };
	}

	initMouseInput();
}



void Input::initMouseInput()
{
	glfwSetCursorPosCallback(window->getAPIWindowPtr(), mouseCursorCallback);


	double xpos, ypos;
	glfwGetCursorPos(this->window->getAPIWindowPtr(), &xpos, &ypos);
	glm::vec2 currentMousePosition{ static_cast<float>(xpos), static_cast<float>(ypos) };
	lastMousePosition = currentMousePosition;
}


void Input::mouseCursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	glm::vec2 currentMousePosition{ static_cast<float>(xpos), static_cast<float>(ypos) };
	mouseDelta = currentMousePosition - lastMousePosition;
	// printVector2(mouseDelta) etc etc 
	lastMousePosition = currentMousePosition;

	CharacterLookEvent event;
	event.LookDirection = mouseDelta;
	SendEvent<CharacterLookEvent>(event);
}


void Input::updateInput()
{
}


// almost the same as before; call this function from outside I guess
// just one difference is we now have the deltaTime thing... goddamn it
void Input::processInput(float deltaTime)
{
	glfwPollEvents();


	if (glfwGetKey(window->getAPIWindowPtr(), GLFW_KEY_Q) == GLFW_PRESS)
		window->setShouldClose();


	auto apiWindowPtr = this->window->getAPIWindowPtr();

	int directionFlags = 0;

	if (glfwGetKey(apiWindowPtr, GLFW_KEY_W) == GLFW_PRESS) {
		directionFlags |= WalkDirections::FORWARD;
	}
	if (glfwGetKey(apiWindowPtr, GLFW_KEY_S) == GLFW_PRESS) {
		directionFlags |= WalkDirections::BACKWARD;
	}
	if (glfwGetKey(apiWindowPtr, GLFW_KEY_A) == GLFW_PRESS) {
		directionFlags |= WalkDirections::STRAFE_LEFT;
	}
	if (glfwGetKey(apiWindowPtr, GLFW_KEY_D) == GLFW_PRESS) {
		directionFlags |= WalkDirections::STRAFE_RIGHT;
	}

	if (directionFlags) 
	{
		CharacterWalkEvent walkEvent;
		walkEvent.DirectionFlags = directionFlags;
		walkEvent.Speed = 1.f;
		SendEvent<CharacterWalkEvent>(walkEvent);
	}
};
