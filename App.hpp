#pragma once

#include <chrono>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <GL/GL.h>
#include <glm/glm.hpp>

#include "Window.hpp"
#include "Input/InputSystem.hpp"

#include "UI/ui.hpp"

#include "Scene.hpp"

class App
{
	Input input;

	Scene scene;
public:
	App()
		: windowObject{ windowWidth, windowHeight, "App" }
	{
		initGL();
		input.setup(windowObject);

		scene.setup();

		ui.Init(windowObject.getAPIWindowPtr());

		loop();
	}

	~App()
	{
		ui.Shutdown();
	}


	void initGL()
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			throw std::exception("Failed to initialize GLAD");
		}

		glEnable(GL_DEPTH_TEST);

		glClearColor(0.f, 0, 0.f, 1.f);
		glViewport(0, 0, windowWidth, windowHeight);
	}

	void loop()
	{
		lastFrameTime = 0.0;

		while (!windowObject.shouldClose())
		{
			deltaTime = glfwGetTime() - lastFrameTime; 
			
			input.processInput(deltaTime);

			auto start = std::chrono::high_resolution_clock::now();

			render();

			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> render_time = end - start;
			glfwSetWindowTitle(windowObject.getAPIWindowPtr(), (std::string( "Render time: " ) + std::to_string(render_time.count()) + " ms").c_str());

			lastFrameTime = glfwGetTime();
		}
	}


	void render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ui.BeginFrame();
		scene.render(windowWidth, windowHeight);
		ui.EndFrame();

		glfwSwapBuffers(windowObject.getAPIWindowPtr());
	}


	static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	const int windowWidth = 1920;
	const int windowHeight = 1080;

	double deltaTime;
	double lastFrameTime;
	glm::vec2 lastMousePosition;
	glm::vec2 mouseDelta;
	Window windowObject;
	// maybe UI should go into scene?
	UI ui;
};
