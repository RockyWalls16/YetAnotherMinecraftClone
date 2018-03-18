/*
 * ComplexChunk.h
 *
 *  Created on: 13 sept. 2017
 *      Author: Valentin
 */

#ifndef CORE_WORLD_LAYEREDCHUNK_H_
#define CORE_WORLD_LAYEREDCHUNK_H_

#include <core/world/AirChunk.h>

class Block;

class ComplexChunk : public AirChunk
{
private:
	short blocks[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];

public:
	ComplexChunk(World& world, int x, int y, int z);
	~ComplexChunk();

	virtual short getBlockAt(int x, int y, int z) override;
	virtual void setBlockAt(Block* block, int x, int y, int z, bool reDraw) override;
	virtual ChunkType getChunkType() override;
	short* getData();
};

#endif /* CORE_WORLD_LAYEREDCHUNK_H_ */
