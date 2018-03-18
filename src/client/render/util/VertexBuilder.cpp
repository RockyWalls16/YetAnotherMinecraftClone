/*
 * VertexBuilder.cpp
 *
 *  Created on: 14 sept. 2017
 *      Author: Valentin
 */

#include <client/render/util/VertexBuilder.h>
#include <util/Logger.h>
#include <algorithm>

VertexBuilder::VertexBuilder(int dataPerIndices, int defaultSize, float increaseRatio) : vertexBufferSize(defaultSize * dataPerIndices), indicesBufferSize(defaultSize), dataPerIndices(dataPerIndices), increaseRatio(increaseRatio), vertexWriteIndex(0), indicesWriteIndex(0), lastIndice(0)
{
	vertexBuffer = (float*)malloc(sizeof(float) * vertexBufferSize);
	indicesBuffer = (unsigned int*)malloc(sizeof(unsigned int) * indicesBufferSize);
}

VertexBuilder::~VertexBuilder()
{
	free(vertexBuffer);
	free(indicesBuffer);
}

void VertexBuilder::addVertices(float* vertices, unsigned int vertexAmount, unsigned int* indices, unsigned int indicesAmount)
{
	// Check buffer is large enough to write new data
	if(indicesAmount + indicesWriteIndex >= indicesBufferSize)
	{
		Warning("VertexBuilder: BufferOverflow - Auto resizing buffer...");

		// Grow buffer size until there is enough room to fit new data
		while(indicesAmount + indicesWriteIndex >= indicesBufferSize)
		{
			indicesBufferSize = (int) (indicesBufferSize * increaseRatio);
			vertexBufferSize = (int) (indicesBufferSize * dataPerIndices);
		}

		// Resize buffer
		indicesBuffer = (unsigned int*) realloc(indicesBuffer, indicesBufferSize * sizeof(unsigned int));
		vertexBuffer = (float*) realloc(vertexBuffer, vertexBufferSize * sizeof(float));
	}

	// Add new values to buffers
	unsigned int verticesAmount = vertexAmount * dataPerIndices;

	std::copy(vertices, vertices + verticesAmount, vertexBuffer + vertexWriteIndex);
	std::copy(indices, indices + indicesAmount, indicesBuffer + indicesWriteIndex);

	// Increase write index
	vertexWriteIndex += verticesAmount;
	indicesWriteIndex += indicesAmount;
}

void VertexBuilder::rewind()
{
	vertexWriteIndex = 0;
	indicesWriteIndex = 0;
	lastIndice = 0;
}

unsigned int* VertexBuilder::getIndicesBuffer() const
{
	return indicesBuffer;
}

float* VertexBuilder::getVertexBuffer() const
{
	return vertexBuffer;
}

unsigned int VertexBuilder::getIndicesWriteIndex() const
{
	return indicesWriteIndex;
}

unsigned int VertexBuilder::getVertexWriteIndex() const
{
	return vertexWriteIndex;
}

unsigned int VertexBuilder::getVBOSize() const
{
	return sizeof(float) * vertexWriteIndex;
}

unsigned int VertexBuilder::getEOBSize() const
{
	return sizeof(unsigned int) * indicesWriteIndex;
}

unsigned int VertexBuilder::getLastIndice() const
{
	return lastIndice;
}

void VertexBuilder::setLastIndice(unsigned int lastIndice)
{
	this->lastIndice = lastIndice;
}
