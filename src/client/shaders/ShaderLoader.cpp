/*
 * ShaderLoader.cpp
 *
 *  Created on: 31 août 2017
 *      Author: Valentin
 */

#include <util/GLHeader.h>
#include <client/shaders/ShaderLoader.h>
#include <util/Logger.h>
#include <fstream>
#include <sstream>

#define SHADER_PATH "Assets/Shaders/"


void ShaderLoader::initShader(Shader& shader)
{
	// Load vertex & fragment shaders
	unsigned int vertexId = loadShaderFile(ShaderType::VERTEX, SHADER_PATH + shader.getShaderName() + ".vs");
	unsigned int fragmentId = loadShaderFile(ShaderType::FRAGMENT, SHADER_PATH + shader.getShaderName() + ".fs");

	// Create the shader program
	unsigned int programId = glCreateProgram();
	glAttachShader(programId, vertexId);
	glAttachShader(programId, fragmentId);
	glLinkProgram(programId);

	int success;
	char infoLog[1024];

	// Check link error
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programId, 1024, NULL, infoLog);
		NLog(LOG_LINE);
		NLog("Shader (" + shader.getShaderName() + ") link error:\n");
		NLog(infoLog);
		NLog(LOG_LINE);
	}

	// Delete shaders
	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);

	shader.init(programId);
}

unsigned int ShaderLoader::loadShaderFile(ShaderType type, std::string path)
{
	// Load file content
	std::string fileContent = "";
	std::ifstream inputStream(path);

	if(inputStream.good())
	{
		inputStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			std::stringstream codeStream;
			codeStream << inputStream.rdbuf();
			inputStream.close();

			fileContent = codeStream.str();

		} catch (std::ifstream::failure& e)
		{
			Error("Failed to load shader from: " + path + " " + e.what());
		}
	}
	else
	{
		Error("Shader file not found: " + path);
	}

	const char* shaderCode = fileContent.c_str();
	unsigned int shaderId;
	int success;
	char infoLog[1024];

	// Create shader
	shaderId = glCreateShader(type == ShaderType::VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
	glShaderSource(shaderId, 1, &shaderCode, NULL);
	glCompileShader(shaderId);

	// Check compile errors
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderId, 1024, NULL, infoLog);
		NLog(LOG_LINE);
		NLog("Shader (" + path + ") compile error:\n");
		NLog(infoLog);
		NLog(LOG_LINE);
	}

	return shaderId;
}
