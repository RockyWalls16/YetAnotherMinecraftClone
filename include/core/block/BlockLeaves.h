#ifndef _CORE_BLOCK_BLOCKLEAVES_H_
#define _CORE_BLOCK_BLOCKLEAVES_H_

#include <core/block/Block.h>

class BlockLeaves : public Block
{
public:

	BlockLeaves(unsigned short id, int textureId) : Block(id, textureId) {};

	virtual bool hideNeighbourFace(Block* neighbour, Side neighbourSide) override
	{
		return false;
	}
};

#endif
