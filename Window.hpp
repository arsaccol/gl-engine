#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace WindowManagement
{
	// This shouldn't be here in this file as it concerns OpenGL stuff
	// But maybe I'm wrong? Since a resize callback is still a concern of
	// the window...
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
}

// Abstraction for window creation and window state maintenance
class Window
{
public:
	Window(int windowWidth, int windowHeight, const char* windowTitle)
		: windowWidth{windowWidth}, windowHeight{windowHeight}
		, title{ windowTitle }
	{
		initAPI();

	}

	~Window()
	{
		glfwTerminate();
	}

	// temporary thingie
	GLFWwindow* getAPIWindowPtr()
	{
		return apiWindow;
	}

	void setShouldClose()
	{
		glfwSetWindowShouldClose(apiWindow, true);
	}

	bool shouldClose()
	{
		return glfwWindowShouldClose(apiWindow);
	}



private:
	void initAPI()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
		//apiWindow = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), nullptr, nullptr);
		apiWindow = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), glfwGetPrimaryMonitor(), nullptr);

		if (apiWindow == nullptr) {
			glfwTerminate();
			throw std::exception("Couldn't create GLFW window");
		}

		glfwMakeContextCurrent(apiWindow);

		// Vsync off
		glfwSwapInterval(0);

		glfwSetFramebufferSizeCallback(apiWindow, WindowManagement::framebufferSizeCallback);
	}

	
private:
	int windowWidth;
	int windowHeight;
	GLFWwindow* apiWindow;
	std::string title;
};

