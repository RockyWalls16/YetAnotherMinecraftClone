#include "client/shaders/PostProcessShader.h"
#include <util/GLHeader.h>

void PostProcessShader::use()
{
	Shader::use();
}

void PostProcessShader::stop()
{
	Shader::stop();
}

void PostProcessShader::bindAttributesAndUniforms()
{
	bindAttribute(0, "aPos");
	bindAttribute(1, "aTex");
}