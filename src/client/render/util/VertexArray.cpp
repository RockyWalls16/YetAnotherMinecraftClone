/*
 * VertexArray.cpp
 *
 *  Created on: 4 sept. 2017
 *      Author: Valentin
 */

#include <util/Logger.h>
#include <client/shaders/StaticShader.h>
#include <client/render/util/VertexArray.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

VertexArray::VertexArray(unsigned int id) : idVAO(id), idEBO(0), dirty(true), useNormals(true)
{
	setIdentity();
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &idVAO);

	if (idEBO != 0)
	{
		glDeleteBuffers(1, &idEBO);
	}

	for (unsigned int idVBO : vbos)
	{
		glDeleteBuffers(1, &idVBO);
	}
}

VertexArray* VertexArray::makeVAO()
{
	// Generate new VAO Id
	unsigned int vaoId;
	glGenVertexArrays(1, &vaoId);

	VertexArray* vao = new VertexArray(vaoId);

	return vao;
}

void VertexArray::drawVAO(int vertexAmount, int offset, int drawShape, bool addModelMatrix)
{
	processNormalMatrix();
	bind();
	if (addModelMatrix && Shader::currentShader != nullptr)
	{
		Shader::currentShader->onDraw(modelMatrix, normalMatrix);
	}
	glDrawArrays(drawShape, offset, vertexAmount);
}

void VertexArray::drawEBO(int vertexAmount, GLvoid* offset, int drawShape, bool addModelMatrix)
{
	processNormalMatrix();
	bind();
	if (addModelMatrix && Shader::currentShader != nullptr)
	{
		Shader::currentShader->onDraw(modelMatrix, normalMatrix);
	}
	glDrawElements(drawShape, vertexAmount, GL_UNSIGNED_INT, (void*) 0);
}

void VertexArray::addVBO(void* buffer, unsigned int bufferSize, int drawType)
{
	unsigned int vboId;

	bind();

	// Create new VBO & bind data
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, buffer, drawType);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	vbos.push_back(vboId);
}

void VertexArray::enableEBO(unsigned int* indices, unsigned int indicesSize, int drawType)
{
	bind();

	glGenBuffers(1, &idEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, drawType);
}

void VertexArray::assignVertexAttrib(unsigned int vboIndex, unsigned int attribIndex, int componentAmount, int componentType, unsigned int stride, GLvoid* offset, bool normalized)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbos[vboIndex]);
	glVertexAttribPointer(attribIndex, componentAmount, componentType, normalized ? GL_TRUE : GL_FALSE, stride, offset);
	glEnableVertexAttribArray(attribIndex);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexArray::assignPositionAttrib(unsigned int vboId, unsigned int attribIndex, unsigned int stride, GLvoid* offset)
{
	assignVertexAttrib(vboId, attribIndex, 3, GL_FLOAT, stride, offset, false);
}

void VertexArray::assign2DPositionAttrib(unsigned int vboId, unsigned int attribIndex, unsigned int stride, GLvoid* offset)
{
	assignVertexAttrib(vboId, attribIndex, 2, GL_FLOAT, stride, offset, false);
}


void VertexArray::assignRGBAttrib(unsigned int vboId, unsigned int attribIndex, unsigned int stride, GLvoid* offset)
{
	assignVertexAttrib(vboId, attribIndex, 3, GL_FLOAT, stride, offset, false);
}

void VertexArray::assignRGBAAttrib(unsigned int vboId, unsigned int attribIndex, unsigned int stride, GLvoid* offset)
{
	assignVertexAttrib(vboId, attribIndex, 4, GL_FLOAT, stride, offset, false);
}

void VertexArray::assignUVAttrib(unsigned int vboId, unsigned int attribIndex, unsigned int stride, GLvoid* offset)
{
	assignVertexAttrib(vboId, attribIndex, 2, GL_FLOAT, stride, offset, false);
}

void VertexArray::bind()
{
	glBindVertexArray(idVAO);
}

void VertexArray::unbind()
{
	glBindVertexArray(0);
}

void VertexArray::setIdentity()
{
	modelMatrix = glm::mat4(1.0F);
	dirty = true;
}

void VertexArray::translate(const glm::vec3& translation)
{
	modelMatrix = glm::translate(modelMatrix, translation);
	dirty = true;
}

void VertexArray::rotate(float amount, const glm::vec3& rotation)
{
	modelMatrix = glm::rotate(modelMatrix, amount, rotation);
	dirty = true;
}

void VertexArray::scale(const glm::vec3& scale)
{
	modelMatrix = glm::scale(modelMatrix, scale);
	dirty = true;
}

void VertexArray::processNormalMatrix()
{
	if (dirty && useNormals)
	{
		normalMatrix = glm::inverseTranspose(glm::mat3(modelMatrix));
		dirty = false;
	}
}

void VertexArray::disableNormals()
{
	useNormals = false;
}
