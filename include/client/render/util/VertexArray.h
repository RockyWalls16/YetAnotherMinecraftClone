/*
 * VertexArray.h
 *
 *  Created on: 4 sept. 2017
 *      Author: Valentin
 */

#ifndef CLIENT_RENDER_UTIL_VERTEXARRAY_H_
#define CLIENT_RENDER_UTIL_VERTEXARRAY_H_

#include <util/GLHeader.h>
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <vector>

class VertexArray
{
private:
	unsigned int idVAO;
	unsigned int idEBO;
	std::vector<unsigned int> vbos;
	glm::mat4 modelMatrix;
	glm::mat3 normalMatrix;
	bool dirty;
	bool useNormals;

public:
	static VertexArray* makeVAO();

	//void addSingleDataVBO(unsigned int index, float* buffer, unsigned int length);
	void bind();
	void unbind();
	void drawVAO(int opaqueVertexAmount, int offset = 0, int drawShape = GL_TRIANGLES, bool addModelMatrix = true);
	void drawEBO(int opaqueVertexAmount, GLvoid* offset = 0, int drawShape = GL_TRIANGLES, bool addModelMatrix = true);
	void addVBO(void* buffer, unsigned int bufferSize, int drawType);
	void enableEBO(unsigned int* indices, unsigned int indicesSize, int drawType);
	void assignVertexAttrib(unsigned int vboId, unsigned int attribIndex, int componentAmount, int componentType, unsigned int stride, GLvoid* offset = 0, bool normalized = false);
	void assignPositionAttrib(unsigned int vboId, unsigned int attribIndex, unsigned int stride, GLvoid* offset = 0);
	void assign2DPositionAttrib(unsigned int vboId, unsigned int attribIndex, unsigned int stride, GLvoid* offset = 0);
	void assignRGBAttrib(unsigned int vboId, unsigned int attribIndex, unsigned int stride, GLvoid* offset = 0);
	void assignRGBAAttrib(unsigned int vboId, unsigned int attribIndex, unsigned int stride, GLvoid* offset = 0);
	void assignUVAttrib(unsigned int vboId, unsigned int attribIndex, unsigned int stride, GLvoid* offset = 0);

	void setIdentity();
	void translate(const glm::vec3& translation);
	void rotate(float amount, const glm::vec3& rotation);
	void scale(const glm::vec3& scale);
	void processNormalMatrix();
	void disableNormals();

	~VertexArray();

private:
	VertexArray(unsigned int idVAO);
};

#endif /* CLIENT_RENDER_UTIL_VERTEXARRAY_H_ */
