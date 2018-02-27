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
	static Block* PLANKS;
	static Block* SLABS;
	static Block* BRICKS;
	static Block* TNT;
	static Block* COBBLE;
	static Block* BEDROCK;
	static Block* SAND;
	static Block* GRAVEL;
	static Block* GRASS;
	static Block* WATER;

	Block(unsigned short id, int textureId);

	short getId();

	virtual int getTexture(Side faceSide);

	virtual bool isVisible();

	virtual bool isSideVisible(Block* neighbour, Side faceSide);

	virtual bool hideNeighbourFace(Block* neighbour, Side neighbourSide);

	virtual RenderLayer getRenderLayer();

	virtual AABB getHitbox(int x, int y, int z);

	virtual bool canCollide();

	virtual bool isSimpleBlock();

	static Block* getBlock(unsigned short id);

	static std::vector<Block*>& getBlockList();
};

#endif