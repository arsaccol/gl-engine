#include "Helper.hpp"

#include <iostream>
#include <glad/glad.h>
#include <GL/gl.h>

void printGLError(const std::string& contextName)
{
	std::cout << "----------------- Error check ----------------" << std::endl;

	do {
		auto errorCode = glGetError();
		std::cout << "Context: " << contextName << std::endl;

		if (errorCode)
			std::cout << "Error code: " << errorCode << std::endl;
		else
			std::cout << "No errors" << std::endl;
	} while (glGetError());

	std::cout << "-----------------------------------------------" << std::endl;
}

void printVector3(const std::string& name, glm::vec3 vector)
{
	printf("%s: %f, %f, %f\n", name.c_str(), vector.x, vector.y, vector.z);
}

void printVector2(const std::string& name, glm::vec2 vector)
{
	printf("%s: %f, %f\n", name.c_str(), vector.x, vector.y);
}
