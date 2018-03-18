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

ComplexChunk::ComplexChunk(World& world, int x, int y, int z) : AirChunk(world, x, y, z), blocks()
{

}

ComplexChunk::~ComplexChunk()
{

}

short ComplexChunk::getBlockAt(int x, int y, int z)
{
	return blocks[AirChunk::getFlatIndex(x, y, z)];
}

void ComplexChunk::setBlockAt(Block* block, int x, int y, int z, bool reDraw)
{
	blocks[AirChunk::getFlatIndex(x, y, z)] = block->getId();

	// Redraw chunk if asked to
	if (reDraw)
	{
		setDirty(block, x, y, z);
	}
}

ChunkType ComplexChunk::getChunkType()
{
	return ChunkType::LAYERED;
}

short * ComplexChunk::getData()
{
	return blocks;
}
