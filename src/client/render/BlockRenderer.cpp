/*
 * BlockRenderer.cpp
 *
 *  Created on: 8 févr. 2018
 *      Author: Valentin
 */

#include <client/render/BlockRenderer.h>
#include <client/textures/TextureLoader.h>
#include <client/textures/TextureCache.h>
#include <client/shaders/ShaderCache.h>
#include <util/Side.h>
#include <util/Logger.h>

#define DATA_PER_VERTEX_ARRAY 36
#define DATA_PER_VERTEX 44
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

	ShaderCache::blockShader->setAtlasCellSize(atlasCellW, atlasCellH);
}

void BlockRenderer::renderBlock(VertexBuilder* vertexBuilder, const shared_ptr<AirChunk>& chunk, Block* blockId, int x, int y, int z)
{
	/*if(shallRenderFace(chunk, blockId, x, y + 1, z, Side::TOP))
	{
		renderFace(vertexBuilder, chunk, blockId, x, y, z, Side::TOP); // Top face
	}

	if(shallRenderFace(chunk, blockId, x, y - 1, z, Side::BOTTOM))
	{
		renderFace(vertexBuilder, chunk, blockId, x, y, z, Side::BOTTOM); // Bottom face
	}

	if(shallRenderFace(chunk, blockId, x + 1, y, z, Side::EAST))
	{
		renderFace(vertexBuilder, chunk, blockId, x, y, z, Side::EAST); // X+ face
	}

	if(shallRenderFace(chunk, blockId, x, y, z - 1, Side::NORTH))
	{
		renderFace(vertexBuilder, chunk, blockId, x, y, z, Side::NORTH); // Z- face
	}

	if(shallRenderFace(chunk, blockId, x - 1, y, z, Side::WEST))
	{
		renderFace(vertexBuilder, chunk, blockId, x, y, z, Side::WEST); // X- face
	}

	if(shallRenderFace(chunk, blockId, x, y, z + 1, Side::SOUTH))
	{
		renderFace(vertexBuilder, chunk, blockId, x, y, z, Side::SOUTH); // Z+ face
	}*/
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

	shared_ptr<AirChunk> other = neighbours[side];
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

void BlockRenderer::renderFace(VertexBuilder* vertexBuilder, Block* blockId, int x, int y, int z, int sX, int sY, int sZ, Side faceSide)
{
	static unsigned int indices[] =
	{
		0, 1, 2,
		1, 3, 2
	};

	// Calculate texture coordinates
	int cellId = blockId->getTexture(faceSide);
	int u = (cellId % ATLAS_COLUMNS);
	int v = ATLAS_COLUMNS - (cellId / ATLAS_COLUMNS) - 1;

	float cubePos[DATA_PER_VERTEX];

	int verticeOffset = (DATA_PER_VERTEX_ARRAY * faceSide);
	int j = 0;
	for(int i = 0; i < DATA_PER_VERTEX; i += VERTEX_STRIDE)
	{
		// Positions
		cubePos[i] = vertices[verticeOffset + j] * sX + x;
		cubePos[i + 1] = vertices[verticeOffset + j + 1] * sY + y;
		cubePos[i + 2] = vertices[verticeOffset + j + 2] * sZ + z;

		// Colors
		cubePos[i + 3] = vertices[verticeOffset + j + 3];
		cubePos[i + 4] = vertices[verticeOffset + j + 4];
		cubePos[i + 5] = vertices[verticeOffset + j + 5];

		// Normals
		cubePos[i + 6] = vertices[verticeOffset + j + 6];
		cubePos[i + 7] = vertices[verticeOffset + j + 7];
		cubePos[i + 8] = vertices[verticeOffset + j + 8];

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

	vertexBuilder->setLastIndice(lastIndice + 6);
	vertexBuilder->addVertices(cubePos, cubeIndices, 6);
}

float BlockRenderer::atlasCellW = 1.0F;
float BlockRenderer::atlasCellH = 1.0F;

float BlockRenderer::vertices[] =
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