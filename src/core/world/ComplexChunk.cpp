/*
 * ComplexChunk.cpp
 *
 *  Created on: 13 sept. 2017
 *      Author: Valentin
 */

#include <core/world/ComplexChunk.h>
#include <util/Logger.h>
#include <core/world/World.h>
#include <core/block/Block.h>

ComplexChunk::ComplexChunk(World* world, int x, int y, int z, short** layers) : AirChunk(world, x, y, z), layers(layers)
{

}

ComplexChunk::~ComplexChunk()
{
	for (int i = 0; i < CHUNK_SIZE; i++)
	{
		delete(layers[i]);
	}
	delete(layers);
}

short ComplexChunk::getBlockAt(int x, int y, int z)
{
	if(x < 0 || y < 0 || z < 0 || x >= CHUNK_SIZE || z >= CHUNK_SIZE || y >= CHUNK_SIZE)
	{
		return 0;
	}

	return layers[y][AirChunk::getFlatIndex(x, z)];
}

void ComplexChunk::setBlockAt(Block* block, int x, int y, int z, bool reDraw)
{
	layers[y][AirChunk::getFlatIndex(x, z)] = block->getId();

	// Redraw chunk if asked to
	if (reDraw && isReady())
	{
		setDirty(block, x, y, z);
	}
}

ChunkType ComplexChunk::getChunkType()
{
	return ChunkType::LAYERED;
}