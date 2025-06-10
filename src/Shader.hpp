#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdio>
#include <map>
#include <utility> // for std::pair


#include "Helper.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp> // for glm::value_ptr
#include "Light.hpp" // Light struct might be used by shader data structs passed here

#include <iostream>


class Shader
{
public:
	Shader(GLenum shaderTarget, const char* sourceFilename)
		: ready{false}
		, shaderTarget{shaderTarget}
		, shaderId{glCreateShader(shaderTarget)}
	{
		std::cout << "Initializing shader \"" << sourceFilename << "\" with ID " << shaderId << std::endl;

		if (loadAndCompileShader(shaderTarget, sourceFilename)) {
			ready = true;
		}
		else {
			// Maybe all the bool thing is redundant if we're throwing in the end?
			throw std::runtime_error{ "Couldn't load and compile shaders" };
		}
	}

	void deleteShader()
	{
		glDeleteShader(shaderId);
	}

	~Shader()
	{
		//////////////////////////////////////////////////////////////////////
		// glDeleteShader is commented-out because, for some reason, 
		// Shader::~Shader is being called when a reference to Shader
		// exits scope in the for-loop in ShaderProgram::prepareProgram after
		// callinig glAttachShader,
		// and this ONLY happens in the first loop iteration, i.e. for the
		// vertex shader as of now.
		//////////////////////////////////////////////////////////////////////

		//glDeleteShader(shaderId);
	}

	GLenum getTarget() const { return shaderTarget; }
	GLuint getShaderId() const { return shaderId; }
	bool isReadyForLinking() const { return ready; }

private:

	bool loadAndCompileShader(GLenum shaderTarget, const char* sourceFilename)
	{
		std::string shaderSource = loadShaderSource(sourceFilename);

		if (shaderSource.empty()) {
			return false;
		}
		else {
			if (!compileShader(shaderSource)) {
				return false;
			}
			else {
				return true;
			}
		}
	}


	std::string loadShaderSource(const char* sourceFilename) 
	{
		std::ifstream sourceFileStream{ sourceFilename, std::ios::in };
		if (sourceFileStream.is_open()) {
			std::stringstream sstr;
			sstr << sourceFileStream.rdbuf();
			sourceFileStream.close();

			return sstr.str();
		}
		else {
			throw std::runtime_error{ "Couldn't open open shader source file" };
		}
	}

	bool compileShader(const std::string& sourceCode) 
	{
		GLint result = GL_FALSE;
		int info_log_length{ 0 };
		char const* source_pointer = sourceCode.c_str();

		glShaderSource(shaderId, 1, &source_pointer, nullptr);
		glCompileShader(shaderId);


		// Check whether compilation was successful
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
		std::cout << "Shader compilation status: " << result << std::endl;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &info_log_length);

		if (info_log_length > 0) {
			std::vector<char> vertex_shader_error_message(info_log_length + 1);
			glGetShaderInfoLog(shaderId, info_log_length, nullptr, &vertex_shader_error_message[0]);
			printf("%s", &vertex_shader_error_message[0]);

			return false;
		}
		else return true;
	}

	GLenum shaderTarget;
	GLuint shaderId;
	bool ready;
};


class ShaderProgram
{
public:
	using ShaderTargetFilenamePairs = std::vector< std::pair< GLenum, const char*> >;

	ShaderProgram(ShaderTargetFilenamePairs pairs)
		: shaderProgramId{glCreateProgram()}
	{
		prepareShaders(pairs);
		printGLError("After prepareShaders");
		prepareProgram();
	}


	void use() const
	{
		glUseProgram(shaderProgramId);
	}

	void disuse() const
	{
		glUseProgram(0);
	}

	void setVec3(const glm::vec3& vector, const char* vectorNameInShader)
	{
		glUseProgram(shaderProgramId);
		glUniform3fv(glGetUniformLocation(shaderProgramId, vectorNameInShader), 1, glm::value_ptr(vector));
		glUseProgram(0);
	}

	void setInt(int value, const char* intNameInShader)
	{
		glUseProgram(shaderProgramId);
		glUniform1i(glGetUniformLocation(shaderProgramId, intNameInShader), value);
		glUseProgram(0);
	}

	template<typename LightType>
	void setLights(const std::vector<LightType>& lights, const std::string& arrayNameInShader)
	{
		glUseProgram(shaderProgramId);

		std::string numLightsName = arrayNameInShader + "Count";
		glUniform1i(glGetUniformLocation(shaderProgramId, numLightsName.c_str()), static_cast<int>(lights.size()));

		for (size_t i = 0; i < lights.size(); ++i)
		{
			std::string uniformName;

			uniformName = arrayNameInShader + "[" + std::to_string(i) + "].position";
			glUniform3fv(glGetUniformLocation(shaderProgramId, uniformName.c_str()), 1, glm::value_ptr(lights[i].position));

			uniformName = arrayNameInShader + "[" + std::to_string(i) + "].diffuse";
			glUniform3fv(glGetUniformLocation(shaderProgramId, uniformName.c_str()), 1, glm::value_ptr(lights[i].diffuse));

			uniformName = arrayNameInShader + "[" + std::to_string(i) + "].ambient";
			glUniform3fv(glGetUniformLocation(shaderProgramId, uniformName.c_str()), 1, glm::value_ptr(lights[i].ambient));

			uniformName = arrayNameInShader + "[" + std::to_string(i) + "].specular";
			glUniform3fv(glGetUniformLocation(shaderProgramId, uniformName.c_str()), 1, glm::value_ptr(lights[i].specular));
		}

		glUseProgram(0);
	}

	void setMatrix4x4(const glm::mat4& matrix, const char* matrixNameInShader)
	{
		glUseProgram(shaderProgramId);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgramId, matrixNameInShader), 1, GL_FALSE, glm::value_ptr(matrix));
		glUseProgram(0);
	}

	void setMatrix3x3(const glm::mat3& matrix, const char* matrixNameInShader)
	{
		glUseProgram(shaderProgramId);
		glUniformMatrix3fv(glGetUniformLocation(shaderProgramId, matrixNameInShader), 1, GL_FALSE, glm::value_ptr(matrix));
		glUseProgram(0);
	}

private:
	void prepareShaders(ShaderTargetFilenamePairs pairs)
	{
		for (auto& pair : pairs) 
		{
			shaders.emplace_back(pair.first, pair.second);
		}
	}

	void prepareProgram() 
	{
		for (const auto& shader : shaders) 
		{
			glAttachShader(shaderProgramId, shader.getShaderId());
		}

		glLinkProgram(shaderProgramId);
	}

	void destroyShaders()
	{
		for (auto& shader : shaders)
			shader.deleteShader();

		shaders.clear();
	}

private:
	std::vector<Shader> shaders;


	GLuint shaderProgramId;
};
