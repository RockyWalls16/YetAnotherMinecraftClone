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
		return neighbour != Block::WATER;
	}

	virtual bool hideNeighbourFace(Block* neighbour, Side neighbourSide) override
	{
		return false;
	}

	virtual RenderLayer getRenderLayer() override
	{
		return RenderLayer::RL_TRANSPARENT;
	}

	virtual AABB getRenderHitbox() override
	{
		return AABB(glm::vec3(0.0F, 0.0F, 0.0F), glm::vec3(1.0F, 0.8F, 1.0F));
	}

	virtual bool isSimpleBlock() override { return false; };
};

#endif
