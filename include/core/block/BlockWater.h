#ifndef _CORE_BLOCK_BLOCKWATER_H_
#define _CORE_BLOCK_BLOCKWATER_H_

#include <core/block/Block.h>
#include <client/render/GameRenderer.h>

class BlockWater : public Block
{
public:

	BlockWater(unsigned short id, int textureId) : Block(id, textureId) {};

	virtual bool isSideVisible(Block* neighbour, Side faceSide) override
	{
		return faceSide == Side::TOP && neighbour != Block::WATER;
	}

	virtual RenderLayer getRenderLayer() override
	{
		return RenderLayer::RL_TRANSPARENT;
	}
};

#endif
