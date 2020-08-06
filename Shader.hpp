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

#include <iostream>


class Shader
{
public:
	Shader(GLenum shaderTarget, const char* sourceFilename)
		: ready{false}
		, shaderTarget{shaderTarget}
		, shaderId{glCreateShader(shaderTarget)}
	{

		if (loadAndCompileShader(shaderTarget, sourceFilename)) {
			ready = true;
		}
		else {
			// Maybe all the bool thing is redundant if we're throwing in the end?
		}
	}

	~Shader()
	{
		glDeleteShader(shaderId);
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
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &info_log_length);

		if (info_log_length > 0) {
			std::vector<char> vertex_shader_error_message(info_log_length + 1);
			glGetShaderInfoLog(shaderId, info_log_length, nullptr, &vertex_shader_error_message[0]);
			printf("%s", &vertex_shader_error_message[0]);

			//throw std::runtime_error(&vertex_shader_error_message[0]);
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
		for (auto& shader : shaders) {
			std::cout << "Shader ID: " << shader.getShaderId() << std::endl;
			glAttachShader(shaderProgramId, shader.getShaderId());
		}

		glLinkProgram(shaderProgramId);
	}

	void destroyShaders()
	{
		shaders.clear();
	}

private:
	std::vector<Shader> shaders;


	GLuint shaderProgramId;
};
