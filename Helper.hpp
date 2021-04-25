#pragma once
#include <string>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#define STRINGIFY(x) \
	#x

void printGLError(const std::string& contextName);

void printVector3(const std::string& name, glm::vec3 vector);

void printVector2(const std::string& name, glm::vec2 vector);

