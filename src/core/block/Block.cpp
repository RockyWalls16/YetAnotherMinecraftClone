#include <core/block/Block.h>
#include <core/block/BlockAir.h>
#include <core/block/BlockGrass.h>
#include <core/block/BlockWater.h>
#include <core/block/BlockLeaves.h>
#include <core/block/BlockLog.h>
#include <core/block/BlockPlant.h>
#include <client/render/GameRenderer.h>
#include <math/AABB.h>

std::vector<Block*> Block::blockList = std::vector<Block*>();

Block* Block::AIR = (new BlockAir(0))->setName("Air");
Block* Block::STONE = (new Block(1, 1))->setName("Stone");
Block* Block::DIRT = (new Block(2, 2))->setName("Dirt");
Block* Block::GRASS = (new BlockGrass(3))->setName("Grass");
Block* Block::SAND = (new Block(4, 18))->setName("Sand");
Block* Block::WATER = (new BlockWater(5, 205))->setName("Water");
Block* Block::LEAVES_OAK = (new BlockLeaves(6, 52))->setName("Oak Leaves");
Block* Block::LEAVES_BIRCH = (new BlockLeaves(7, 26))->setName("Birhc Leaves");
Block* Block::WOOD_OAK_LOG = (new BlockLog(8, 20, 21))->setName("Oak Log");
Block* Block::WOOD_BIRCH_LOG = (new BlockLog(9, 117, 21))->setName("Birch Log");
Block* Block::TALL_GRASS = (new BlockPlant(10, 39))->setName("Tall Grass");
Block* Block::FLOWER_ROSE = (new BlockPlant(11, 12))->setName("Rose");
Block* Block::FLOWER_DANDELION = (new BlockPlant(12, 13))->setName("Dandelion");
Block* Block::SUGAR_CANE = (new  BlockPlant(13, 73))->setName("Sugar Cane");
Block* Block::RED_MUSHROOM = (new BlockPlant(14, 28))->setName("Red mushroom");
Block* Block::BROWN_MUSHROOM = (new BlockPlant(15, 29))->setName("Brown mushroom");

Block::Block(unsigned short id, int textureId)
{
	this->id = id;
	this->textureId = textureId;
	blockList.insert(blockList.begin() + id, this);
}

Block* Block::getBlock(unsigned short id)
{
	return blockList[(int) id];
}

std::vector<Block*>& Block::getBlockList()
{
	return blockList;
}

short Block::getId()
{
	return id;
}

Block * Block::setName(const std::string & name)
{
	this->name = name;
	return this;
}

std::string Block::getName()
{
	return name;
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

RenderType Block::getRenderType()
{
	return RenderType::BLOCK;
}
