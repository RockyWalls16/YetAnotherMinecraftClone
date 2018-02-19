/*
 * LayeredChunk.h
 *
 *  Created on: 13 sept. 2017
 *      Author: Valentin
 */

#ifndef CORE_WORLD_LAYEREDCHUNK_H_
#define CORE_WORLD_LAYEREDCHUNK_H_

#include <core/world/AirChunk.h>

class LayeredChunk : public AirChunk
{
private:
	short** layers;
	short layersTypeMask;

public:
	LayeredChunk(World* world, int x, int y, int z, short** layers);
	~LayeredChunk();

	virtual short getBlockAt(int x, int y, int z) override;
	virtual void setBlockAt(short block, int x, int y, int z) override;
	virtual ChunkType getChunkType() override;

	bool isSimpleLayer(int y);
	void setAsSimpleLayer(int y, short block);
	void setAsComplexLayer(int y, short* layerBlocks);
};

#endif /* CORE_WORLD_LAYEREDCHUNK_H_ */
