#pragma once

#include <chrono>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <GL/GL.h>

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
		double dt = 1 / desiredFrameRate;

		double currentTime = glfwGetTime();
		double accumulator = 0.0;

		while (!windowObject.shouldClose())
		{
			double newTime = glfwGetTime();
			double frameTime = newTime - currentTime;
			currentTime = newTime;

			accumulator += frameTime;


			while (accumulator >= dt)
			{
				input.processInput(dt);

				accumulator -= dt;
			}

			auto start = std::chrono::high_resolution_clock::now();

			render();

			auto end = std::chrono::high_resolution_clock::now();
			render_time = end - start;
			//glfwSetWindowTitle(windowObject.getAPIWindowPtr(), (std::string( "Render time: " ) + std::to_string(render_time.count()) + " ms").c_str());
		}
	}


	void render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ui.BeginFrame();
		scene.render(windowWidth, windowHeight);

		ImGui::Begin("Frame time");
		ImGui::Text("Render time: %lf   FPS: %lf", render_time, 1 / render_time.count() * 1000);
		ImGui::End();

		ui.EndFrame();

		// Legend has it that glFlush should be used "in production" for better performance,
		// as glFlush returns immediately whereas glFinish blocks until render is complete
		//
		// I'm not sure if this is a correct measurement, but we use glFinish in order to
		// measure the total render time rather than just move on to the next frame before
		// the current one is finished
		glFinish();
		//glfwSwapBuffers(windowObject.getAPIWindowPtr());
	}


	static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	const int windowWidth = 1920;
	const int windowHeight = 1080;

	std::chrono::duration<double, std::milli> render_time{};


	double desiredFrameRate{60.0};
	double deltaTime;
	double lastFrameTime;
	glm::vec2 lastMousePosition;
	glm::vec2 mouseDelta;
	Window windowObject;
	// maybe UI should go into scene?
	UI ui;
};
