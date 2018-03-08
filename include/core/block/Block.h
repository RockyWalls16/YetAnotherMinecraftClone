#ifndef BLOCK_H_
#define BLOCK_H_

#include <vector>
#include <client/render/WorldRenderer.h>
#include <util/Side.h>

enum RenderLayer;
class AABB;

class Block
{
private:
	static std::vector<Block*> blockList;
	unsigned short id;
	unsigned int textureId;

public:

	static Block* AIR;
	static Block* STONE;
	static Block* DIRT;
	static Block* GRASS;
	static Block* SAND;
	static Block* WATER;
	static Block* LEAVES;

	Block(unsigned short id, int textureId);

	short getId();

	virtual int getTexture(Side faceSide);

	virtual bool isVisible();

	virtual bool isSideVisible(Block* neighbour, Side faceSide);

	virtual bool hideNeighbourFace(Block* neighbour, Side neighbourSide);

	virtual RenderLayer getRenderLayer();

	virtual AABB getHitbox(int x, int y, int z);

	virtual AABB getRenderHitbox();

	virtual bool canCollide();

	virtual bool isSimpleBlock();

	static Block* getBlock(unsigned short id);

	static std::vector<Block*>& getBlockList();
};

#endif