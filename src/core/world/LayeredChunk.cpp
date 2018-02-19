/*
 * LayeredChunk.cpp
 *
 *  Created on: 13 sept. 2017
 *      Author: Valentin
 */

#include <core/world/LayeredChunk.h>
#include <util/Logger.h>

LayeredChunk::LayeredChunk(World* world, int x, int y, int z, short** layers) : AirChunk(world, x, y, z), layers(layers), layersTypeMask(0)
{

}

LayeredChunk::~LayeredChunk()
{
	for (int i = 0; i < CHUNK_SIZE; i++)
	{
		delete(layers[i]);
	}
	delete(layers);
}

short LayeredChunk::getBlockAt(int x, int y, int z)
{
	if(x < 0 || y < 0 || z < 0 || x >= CHUNK_SIZE || z >= CHUNK_SIZE || y >= CHUNK_SIZE)
	{
		return 0;
	}

	return layers[y][AirChunk::getFlatIndex(x, z)];
}

void LayeredChunk::setBlockAt(short block, int x, int y, int z)
{
	layers[y][AirChunk::getFlatIndex(x, z)] = block;
}

ChunkType LayeredChunk::getChunkType()
{
	return ChunkType::LAYERED;
}

bool LayeredChunk::isSimpleLayer(int y)
{
	return (layersTypeMask >> y) & 1;
}

void LayeredChunk::setAsSimpleLayer(int y, short block)
{
	layersTypeMask |= 1 << y;
}

void LayeredChunk::setAsComplexLayer(int y, short * layerBlocks)
{
	layersTypeMask &= ~(1 << y);
}
