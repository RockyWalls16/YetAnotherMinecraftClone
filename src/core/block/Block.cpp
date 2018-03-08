#include <core/block/Block.h>
#include <core/block/BlockAir.h>
#include <core/block/BlockGrass.h>
#include <core/block/BlockWater.h>
#include <core/block/BlockLeaves.h>
#include <client/render/GameRenderer.h>
#include <math/AABB.h>

std::vector<Block*> Block::blockList = std::vector<Block*>();

Block* Block::AIR = new BlockAir(0);
Block* Block::STONE = new Block(1, 1);
Block* Block::DIRT = new Block(2, 2);
Block* Block::GRASS = new BlockGrass(3);
Block* Block::SAND = new Block(4, 18);
Block* Block::WATER = new BlockWater(5, 205);
Block* Block::LEAVES = new BlockLeaves(6, 52);

Block::Block(unsigned short id, int textureId)
{
	this->id = id;
	this->textureId = textureId;
	blockList.insert(blockList.begin() + id, this);
}

Block* Block::getBlock(unsigned short id)
{
	return blockList[id];
}

std::vector<Block*>& Block::getBlockList()
{
	return blockList;
}

short Block::getId()
{
	return id;
}

int Block::getTexture(Side faceSide)
{
	return textureId;
}

bool Block::isVisible()
{
	return true;
}

bool Block::isSideVisible(Block* neighbour, Side faceSide)
{
	return isVisible();
}

bool Block::hideNeighbourFace(Block* neighbour, Side neighbourSide)
{
	return isSideVisible(neighbour, SideUtil::opposite[neighbourSide]);
}

RenderLayer Block::getRenderLayer()
{
	return RenderLayer::RL_OPAQUE;
}

AABB Block::getHitbox(int x, int y, int z)
{
	return AABB(glm::vec3(x, y, z), glm::vec3(1.0F + x, 1.0F + y, 1.0F + z));
}

AABB Block::getRenderHitbox()
{
	return AABB(glm::vec3(0.0F, 0.0F, 0.0F), glm::vec3(1.0F, 1.0F, 1.0F));
}

bool Block::canCollide()
{
	return true;
}

bool Block::isSimpleBlock()
{
	return true;
}
