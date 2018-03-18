/*
 * BlockRenderer.cpp
 *
 *  Created on: 8 févr. 2018
 *      Author: Valentin
 */

#include <core/block/Block.h>
#include <client/render/BlockRenderer.h>
#include <client/textures/TextureLoader.h>
#include <client/textures/TextureCache.h>
#include <client/shaders/ShaderCache.h>
#include <util/Side.h>
#include <util/Logger.h>

#define DATA_PER_FACE_ARRAY 36
#define DATA_PER_FACE 44
#define VERTEX_STRIDE 11
#define VERTEX_STRIDE_ARRAY 9

void BlockRenderer::initBlockRenderer()
{
	// Load block texture
	TextureCache::blockTexture = TextureLoader::loadTexture("minecraft");
	TextureCache::blockSpecularTexture = TextureLoader::loadTexture("minecraft_specular");
	int texW = TextureCache::blockTexture->getTextureWidth();
	int texH = TextureCache::blockTexture->getTextureHeight();

	// Calculate one cell UV
	atlasCellW = (texW / ATLAS_COLUMNS) / (float) texW;
	atlasCellH = (texH / ATLAS_ROWS) / (float) texH;

	ShaderCache::deferredBlockShader->setAtlasCellSize(atlasCellW, atlasCellH);
	ShaderCache::forwardBlockShader->setAtlasCellSize(atlasCellW, atlasCellH);
}

bool BlockRenderer::isSideVisible(const shared_ptr<AirChunk>& chunk, shared_ptr<AirChunk> neighbours[6], Block * blockId, int x, int y, int z, Side faceSide)
{
	return shallRenderFace(chunk, neighbours, blockId, x + SideUtil::xValue[faceSide], y + SideUtil::yValue[faceSide], z + SideUtil::zValue[faceSide], faceSide);
}

bool BlockRenderer::shallRenderFace(const shared_ptr<AirChunk>& chunk, shared_ptr<AirChunk> neighbours[6], Block * blockId, int x, int y, int z, Side faceSide)
{
	Block* neighbour = Block::getBlock(getBlockAtWithNeighbours(chunk, neighbours, faceSide, x, y, z));
	return blockId->isSideVisible(neighbour, faceSide) && !neighbour->hideNeighbourFace(blockId, faceSide);
}

short BlockRenderer::getBlockAtWithNeighbours(const shared_ptr<AirChunk>& chunk, shared_ptr<AirChunk> neighbours[6], Side side, int x, int y, int z)
{
	if (x >= 0 && y >= 0 && z >= 0 && x < CHUNK_SIZE && z < CHUNK_SIZE && y < CHUNK_SIZE)
	{
		return chunk->getBlockAt(x, y, z);
	}

	const shared_ptr<AirChunk>& other = neighbours[side];
	if (other)
	{
		switch (side)
		{
		case TOP: return other->getBlockAt(x, 0, z); //Y+
		case BOTTOM: return other->getBlockAt(x, CHUNK_SIZE - 1, z); //Y-
		case EAST: return other->getBlockAt(0, y, z);  // X+
		case WEST: return other->getBlockAt(CHUNK_SIZE - 1, y, z); // X-
		case SOUTH: return other->getBlockAt(x, y, 0); // Z+
		case NORTH: return other->getBlockAt(x, y, CHUNK_SIZE - 1);  // Z-
		}
	}

	Warning("Trying to mesh a not ready chunk !");
	return 0;
}

void BlockRenderer::renderFace(VertexBuilder* vertexBuilder, Block* blockId, AABB& renderBox, int x, int y, int z, int sX, int sY, int sZ, Side faceSide)
{
	// Calculate texture coordinates
	int cellId = blockId->getTexture(faceSide);
	int u = (cellId % ATLAS_COLUMNS);
	int v = ATLAS_COLUMNS - (cellId / ATLAS_COLUMNS) - 1;

	float cubePos[DATA_PER_FACE];

	int verticeOffset = (DATA_PER_FACE_ARRAY * faceSide);
	int j = 0;
	for(int i = 0; i < DATA_PER_FACE; i += VERTEX_STRIDE)
	{
		// Positions
		cubePos[i] = blockVertices[verticeOffset + j] * sX * renderBox.endPos.x + x + renderBox.startPos.x;
		cubePos[i + 1] = blockVertices[verticeOffset + j + 1] * sY * renderBox.endPos.y + y + renderBox.startPos.y;
		cubePos[i + 2] = blockVertices[verticeOffset + j + 2] * sZ * renderBox.endPos.z + z + renderBox.startPos.z;

		// Colors
		cubePos[i + 3] = blockVertices[verticeOffset + j + 3];
		cubePos[i + 4] = blockVertices[verticeOffset + j + 4];
		cubePos[i + 5] = blockVertices[verticeOffset + j + 5];

		// Normals
		cubePos[i + 6] = blockVertices[verticeOffset + j + 6];
		cubePos[i + 7] = blockVertices[verticeOffset + j + 7];
		cubePos[i + 8] = blockVertices[verticeOffset + j + 8];

		// Atlas pos
		cubePos[i + 9] = u * atlasCellW;
		cubePos[i + 10] = v * atlasCellH;

		j += VERTEX_STRIDE_ARRAY;
	}

	// Set indices
	unsigned int cubeIndices[6];
	unsigned int lastIndice = vertexBuilder->getLastIndice();
	for(j = 0; j < 6; j++)
	{
		cubeIndices[j] = indices[j] + lastIndice;
	}

	vertexBuilder->setLastIndice(lastIndice + 4);
	vertexBuilder->addVertices(cubePos, 4, cubeIndices, 6);
}

void BlockRenderer::renderSimpleFace(VertexBuilder * vertexBuilder, Block * blockId, AABB & renderBox, int x, int y, int z, Side faceSide)
{
	// Calculate texture coordinates
	int cellId = blockId->getTexture(faceSide);
	int u = (cellId % ATLAS_COLUMNS);
	int v = ATLAS_COLUMNS - (cellId / ATLAS_COLUMNS) - 1;

	float cubePos[DATA_PER_FACE];

	int verticeOffset = (DATA_PER_FACE_ARRAY * faceSide);
	int j = 0;
	for (int i = 0; i < DATA_PER_FACE; i += VERTEX_STRIDE)
	{
		// Positions
		cubePos[i] = blockVertices[verticeOffset + j] * renderBox.endPos.x + x + renderBox.startPos.x;
		cubePos[i + 1] = blockVertices[verticeOffset + j + 1] * renderBox.endPos.y + y + renderBox.startPos.y;
		cubePos[i + 2] = blockVertices[verticeOffset + j + 2] * renderBox.endPos.z + z + renderBox.startPos.z;

		// Colors
		cubePos[i + 3] = blockVertices[verticeOffset + j + 3];
		cubePos[i + 4] = blockVertices[verticeOffset + j + 4];
		cubePos[i + 5] = blockVertices[verticeOffset + j + 5];

		// Normals
		cubePos[i + 6] = blockVertices[verticeOffset + j + 6];
		cubePos[i + 7] = blockVertices[verticeOffset + j + 7];
		cubePos[i + 8] = blockVertices[verticeOffset + j + 8];

		// Atlas pos
		cubePos[i + 9] = u * atlasCellW;
		cubePos[i + 10] = v * atlasCellH;

		j += VERTEX_STRIDE_ARRAY;
	}

	// Set indices
	unsigned int cubeIndices[6];
	unsigned int lastIndice = vertexBuilder->getLastIndice();
	for (j = 0; j < 6; j++)
	{
		cubeIndices[j] = indices[j] + lastIndice;
	}

	vertexBuilder->setLastIndice(lastIndice + 4);
	vertexBuilder->addVertices(cubePos, 4, cubeIndices, 6);
}

void BlockRenderer::renderBlock(VertexBuilder * vertexBuilder, Block * blockId, RenderType renderType, int x, int y, int z)
{
	int cellId = blockId->getTexture(Side::TOP);
	int u = (cellId % ATLAS_COLUMNS);
	int v = ATLAS_COLUMNS - (cellId / ATLAS_COLUMNS) - 1;

	switch (renderType)
	{
		case RenderType::CROSS:
		{
			// Vertices
			float cubePos[DATA_PER_FACE * 4];
			for (int i = 0, j = 0; i < DATA_PER_FACE * 4; i += VERTEX_STRIDE)
			{
				// Positions
				cubePos[i] = crossVertices[j] + x;
				cubePos[i + 1] = crossVertices[j + 1] + y;
				cubePos[i + 2] = crossVertices[j + 2] + z;

				// Colors
				cubePos[i + 3] = crossVertices[j + 3];
				cubePos[i + 4] = crossVertices[j + 4];
				cubePos[i + 5] = crossVertices[j + 5];

				// Normals
				cubePos[i + 6] = crossVertices[j + 6];
				cubePos[i + 7] = crossVertices[j + 7];
				cubePos[i + 8] = crossVertices[j + 8];

				// Atlas pos
				cubePos[i + 9] = u * atlasCellW;
				cubePos[i + 10] = v * atlasCellH;

				j += VERTEX_STRIDE_ARRAY;
			}

			// Indices
			unsigned int cubeIndices[24];
			unsigned int lastIndice = vertexBuilder->getLastIndice();
			for (int side = 0; side < 4; side++)
			{
				for (int j = 0; j < 6; j++)
				{
					cubeIndices[j + side * 6] = indices[j] + lastIndice;
				}
				lastIndice += 4;
			}

			vertexBuilder->setLastIndice(lastIndice);
			vertexBuilder->addVertices(cubePos, 16, cubeIndices, 24);
		}
	}
}

float BlockRenderer::atlasCellW = 1.0F;
float BlockRenderer::atlasCellH = 1.0F;

unsigned int BlockRenderer::indices[] =
{
	0, 1, 2,
	1, 3, 2
};

float BlockRenderer::blockVertices[] =
{
	//Top Face - Positions, Colors, Normal
	0.0F, 1.0F, 0.0F, //p
	1.0F, 1.0F, 1.0F, //c
	0.0F, 1.0F, 0.0F, //n

	0.0F, 1.0F, 1.0F, //p
	1.0F, 1.0F, 1.0F, //c
	0.0F, 1.0F, 0.0F, //n

	1.0F, 1.0F, 0.0F, //p
	1.0F, 1.0F, 1.0F, //c
	0.0F, 1.0F, 0.0F, //n

	1.0F, 1.0F, 1.0F, //p
	1.0F, 1.0F, 1.0F, //c
	0.0F, 1.0F, 0.0F, //n

	//Bottom Face - Positions, Colors, Normal
	0.0F, 0.0F, 0.0F, //p
	1.0F, 1.0F, 1.0F, //c
	0.0F, -1.0F, 0.0F,//n

	1.0F, 0.0F, 0.0F, //p
	1.0F, 1.0F, 1.0F, //c
	0.0F, -1.0F, 0.0F,//n

	0.0F, 0.0F, 1.0F, //p
	1.0F, 1.0F, 1.0F, //c
	0.0F, -1.0F, 0.0F,//n

	1.0F, 0.0F, 1.0F, //p
	1.0F, 1.0F, 1.0F, //c
	0.0F, -1.0F, 0.0F,//n

	//X+ Face - Positions, Colors, Normal
	1.0F, 1.0F, 0.0F, //p
	1.0F, 1.0F, 1.0F, //c
	1.0F, 0.0F, 0.0F, //n

	1.0F, 1.0F, 1.0F, //p
	1.0F, 1.0F, 1.0F, //c
	1.0F, 0.0F, 0.0F, //n

	1.0F, 0.0F, 0.0F, //p
	1.0F, 1.0F, 1.0F, //c
	1.0F, 0.0F, 0.0F, //n

	1.0F, 0.0F, 1.0F, //p
	1.0F, 1.0F, 1.0F, //c
	1.0F, 0.0F, 0.0F, //n

	//Z- Face - Positions, Colors, Normal
	0.0F, 1.0F, 0.0F, //p
	1.0F, 1.0F, 1.0F, //c
	0.0F, 0.0F, -1.0F,//n

	1.0F, 1.0F, 0.0F, //p
	1.0F, 1.0F, 1.0F, //c
	0.0F, 0.0F, -1.0F,//n

	0.0F, 0.0F, 0.0F, //p
	1.0F, 1.0F, 1.0F, //c
	0.0F, 0.0F, -1.0F,//n

	1.0F, 0.0F, 0.0F, //p
	1.0F, 1.0F, 1.0F, //c
	0.0F, 0.0F, -1.0F,//n

	//X- Face - Positions, Colors, Normal
	0.0F, 1.0F, 0.0F, //p
	1.0F, 1.0F, 1.0F, //c
	-1.0F, 0.0F, 0.0F,//n

	0.0F, 0.0F, 0.0F, //p
	1.0F, 1.0F, 1.0F, //c
	-1.0F, 0.0F, 0.0F,//n

	0.0F, 1.0F, 1.0F, //p
	1.0F, 1.0F, 1.0F, //c
	-1.0F, 0.0F, 0.0F,//n

	0.0F, 0.0F, 1.0F, //p
	1.0F, 1.0F, 1.0F, //c
	-1.0F, 0.0F, 0.0F,//n

	//Z+ Face - Positions, Colors, Normal
	0.0F, 1.0F, 1.0F, //p
	1.0F, 1.0F, 1.0F, //c
	0.0F, 0.0F, 1.0F, //n

	0.0F, 0.0F, 1.0F, //p
	1.0F, 1.0F, 1.0F, //c
	0.0F, 0.0F, 1.0F, //n

	1.0F, 1.0F, 1.0F, //p
	1.0F, 1.0F, 1.0F, //c
	0.0F, 0.0F, 1.0F, //n

	1.0F, 0.0F, 1.0F, //p
	1.0F, 1.0F, 1.0F, //c
	0.0F, 0.0F, 1.0F, //n
};

float BlockRenderer::crossVertices[] =
{
	//1st Face - Positions, Colors, Normal
	0.0F, 1.0F, 0.0F, //p
	1.0F, 1.0F, 1.0F, //c
	0.5F, 0.0F, -0.5F, //n

	0.0F, 0.0F, 0.0F, //p
	1.0F, 1.0F, 1.0F, //c
	0.5F, 0.0F, -0.5F, //n

	1.0F, 1.0F, 1.0F, //p
	1.0F, 1.0F, 1.0F, //c
	0.5F, 0.0F, -0.5F, //n

	1.0F, 0.0F, 1.0F, //p
	1.0F, 1.0F, 1.0F, //c
	0.5F, 0.0F, -0.5F, //n

	//2nd Face - Positions, Colors, Normal
	0.0F, 0.0F, 1.0F, //p
	1.0F, 1.0F, 1.0F, //c
	-0.5F, 0.0F, -0.5F,//n

	1.0F, 0.0F, 0.0F, //p
	1.0F, 1.0F, 1.0F, //c
	-0.5F, 0.0F, -0.5F,//n

	0.0F, 1.0F, 1.0F, //p
	1.0F, 1.0F, 1.0F, //c
	-0.5F, 0.0F, -0.5F,//n

	1.0F, 1.0F, 0.0F, //p
	1.0F, 1.0F, 1.0F, //c
	-0.5F, 0.0F, -0.5F,//n

	//3rd Face - Positions, Colors, Normal
	0.0F, 0.0F, 0.0F, //p
	1.0F, 1.0F, 1.0F, //c
	0.5F, 0.0F, 0.5F,//n

	0.0F, 1.0F, 0.0F, //p
	1.0F, 1.0F, 1.0F, //c
	0.5F, 0.0F, 0.5F,//n

	1.0F, 0.0F, 1.0F, //p
	1.0F, 1.0F, 1.0F, //c
	0.5F, 0.0F, 0.5F,//n

	1.0F, 1.0F, 1.0F, //p
	1.0F, 1.0F, 1.0F, //c
	0.5F, 0.0F, 0.5F,//n

	//Z+ Face - Positions, Colors, Normal
	0.0F, 0.0F, 1.0F, //p
	1.0F, 1.0F, 1.0F, //c
	-0.5F, 0.0F, 0.5F, //n

	0.0F, 1.0F, 1.0F, //p
	1.0F, 1.0F, 1.0F, //c
	-0.5F, 0.0F, 0.5F, //n

	1.0F, 0.0F, 0.0F, //p
	1.0F, 1.0F, 1.0F, //c
	-0.5F, 0.0F, 0.5F, //n

	1.0F, 1.0F, 0.0F, //p
	1.0F, 1.0F, 1.0F, //c
	-0.5F, 0.0F, 0.5F, //n
};