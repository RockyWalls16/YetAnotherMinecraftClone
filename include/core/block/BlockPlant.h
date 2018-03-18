#ifndef _CORE_BLOCK_BLOCKPLANT_H_
#define _CORE_BLOCK_BLOCKPLANT_H_

#include <core/block/Block.h>
#include <client/render/BlockRenderer.h>

class BlockPlant : public Block
{
public:

	BlockPlant(unsigned short id, int textureId) : Block(id, textureId) {}

	virtual bool hideNeighbourFace(Block* neighbour, Side neighbourSide) override
	{
		return false;
	}

	virtual bool canCollide() override { return false; }

	virtual RenderType getRenderType() override
	{
		return RenderType::CROSS;
	}
};

#endif
