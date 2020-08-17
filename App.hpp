#pragma once

#include <stdexcept>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <GL/GL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // for glm::matrix4

#include "Shader.hpp"
#include "Mesh.hpp"
#include "Helper.hpp"

#include "Camera.hpp"


class App
{
	std::unique_ptr<ShaderProgram> shaderProgram;
	std::unique_ptr<Mesh> triangleMesh;

public:
	App()
	{

		initWindow();


		shaderProgram = std::make_unique<ShaderProgram>(ShaderProgram::ShaderTargetFilenamePairs{
				{GL_VERTEX_SHADER, "vertex_shader.vert"}, 
				{GL_FRAGMENT_SHADER, "fragment_shader.frag"} 
		});


		printGLError("After shader setup");

		triangleMesh = std::make_unique<Mesh>(
			std::vector<Vertex>{
				Vertex{ {0.5f, 0.5f, 0.0f}, {0.f, 0.f}, {1.f, 0.f, 0.f} },
				Vertex{ {0.5f, -0.5f, 0.0f}, {0.f, 0.f}, {0.f, 1.f, 0.f} },
				Vertex{ {-0.5f, 0.5f, 0.0f}, {0.f, 0.f}, {0.f, 0.f, 1.f} },
			},
			std::vector<unsigned int>{ 0, 1, 2 }
		);


		//ShaderProgram shaderProgram{ { 
		//	{GL_VERTEX_SHADER, "vertex_shader.vert"}, 
		//	{GL_FRAGMENT_SHADER, "fragment_shader.frag" } 
		//} };

		loop();
	}

	void initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		window = glfwCreateWindow(windowWidth, windowHeight, "App", nullptr, nullptr);
		if (window == nullptr) {
			glfwTerminate();
			throw std::exception("Couldn't create GLFW window");
		}

		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			throw std::exception("Failed to initialize GLAD");
		}

		glClearColor(1.f, 0, 1.f, 1.f);
		glViewport(0, 0, windowWidth, windowHeight);
	}

	void loop()
	{
		while (!glfwWindowShouldClose(window))
		{
			processInput();

			render();

			glfwPollEvents();
		}
	}

	void processInput()
	{
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}

		Camera::WalkDirection walkDirection{ Camera::WalkDirection::NO_WALK };

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			walkDirection = Camera::WalkDirection::FORWARD;
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			walkDirection = Camera::WalkDirection::BACKWARD;
		else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			walkDirection = Camera::WalkDirection::STRAFE_LEFT;
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			walkDirection = Camera::WalkDirection::STRAFE_RIGHT;
		camera.walk(walkDirection);

	}

	void render()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 view = camera.getMatrix();

		glm::mat4 projection = glm::perspective<float>(glm::radians(60.f), (float)windowWidth / (float)windowHeight, 1.f, 100.f);
		glm::mat4 view_projection =  projection * view;

		shaderProgram->setMatrix4x4(view_projection, "MVP");

		triangleMesh->draw(*shaderProgram);

		glfwSwapBuffers(window);
	}


	static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}


	~App()
	{
		glfwTerminate();
	}

	const int windowWidth = 800;
	const int windowHeight = 600;


	glm::vec2 mouseDelta;
	GLFWwindow* window;
	Camera camera{ glm::vec3{0, 0, 3}, glm::vec3{0, 0, -1} };
};
