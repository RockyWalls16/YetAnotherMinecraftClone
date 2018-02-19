/*
 * Shader.cpp
 *
 *  Created on: 31 août 2017
 *      Author: Valentin
 */

#include <util/TimeManager.h>
#include <client/shaders/Shader.h>
#include <util/GLHeader.h>
#include <util/Logger.h>

Shader* Shader::currentShader = nullptr;

Shader::Shader(const std::string& name) : shaderName(name), programId(0), uniformTimeLocation(0)
{}

Shader::~Shader()
{
	glDeleteShader(programId);
}


void Shader::init(unsigned int id)
{
	programId = id;

	bindAttributesAndUniforms();
}

void Shader::use()
{
	Shader::currentShader = this;
	glUseProgram(programId);
}

void Shader::stop()
{
	Shader::currentShader = nullptr;
	glUseProgram(0);
}

void Shader::bindAttribute(unsigned int index, const std::string& attributeName)
{
	glBindAttribLocation(programId, index, attributeName.c_str());
}

void Shader::bindUniformLocation(const std::string& uniformName, int* locationOutput)
{
	*locationOutput = glGetUniformLocation(programId, uniformName.c_str());

	if (*locationOutput == -1)
	{
		Error("Shader (" + shaderName + "): Uniform " + uniformName + " not found !");
	}
}

const std::string& Shader::getShaderName() const
{
	return shaderName;
}
