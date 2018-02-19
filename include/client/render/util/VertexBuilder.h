/*
 * VertexBuilder.h
 *
 *  Created on: 14 sept. 2017
 *      Author: Valentin
 */

#ifndef CLIENT_RENDER_UTIL_VERTEXBUILDER_H_
#define CLIENT_RENDER_UTIL_VERTEXBUILDER_H_

class VertexBuilder
{
private:
	float* vertexBuffer;
	unsigned int* indicesBuffer;
	unsigned int vertexBufferSize;
	unsigned int indicesBufferSize;
	unsigned int dataPerIndices;
	float increaseRatio;
	unsigned int vertexWriteIndex;
	unsigned int indicesWriteIndex;
	unsigned int lastIndice;
public:
	VertexBuilder(int dataPerIndices, int defaultSize = 10000, float increaseRatio = 1.5F);
	~VertexBuilder();
	void rewind();
	void addVertices(float* vertices, unsigned int* indices, unsigned int vertexAmount);

	unsigned int* getIndicesBuffer() const;
	float* getVertexBuffer() const;
	unsigned int getIndicesWriteIndex() const;
	unsigned int getVertexWriteIndex() const;
	unsigned int getVBOSize() const;
	unsigned int getEOBSize() const;
	unsigned int getLastIndice() const;
	void setLastIndice(unsigned int lastIndice);
};

#endif /* CLIENT_RENDER_UTIL_VERTEXBUILDER_H_ */
