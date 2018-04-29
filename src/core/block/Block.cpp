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
Block* Block::LEAVES_OAK = (new BlockLeaves(6, 52))->setName("Oak leaves");
Block* Block::LEAVES_BIRCH = (new BlockLeaves(7, 26))->setName("Birch leaves");
Block* Block::LEAVES_PINE = (new BlockLeaves(8, 132))->setName("Pine leaves");
Block* Block::WOOD_OAK_LOG = (new BlockLog(9, 20, 21))->setName("Oak log");
Block* Block::WOOD_BIRCH_LOG = (new BlockLog(10, 117, 21))->setName("Birch log");
Block* Block::WOOD_PINE_LOG = (new BlockLog(11, 116, 21))->setName("Pine log");
Block* Block::TALL_GRASS = (new BlockPlant(12, 39))->setName("Tall grass");
Block* Block::FLOWER_ROSE = (new BlockPlant(13, 12))->setName("Rose");
Block* Block::FLOWER_DANDELION = (new BlockPlant(14, 13))->setName("Dandelion");
Block* Block::SUGAR_CANE = (new  BlockPlant(15, 73))->setName("Sugar cane");
Block* Block::RED_MUSHROOM = (new BlockPlant(16, 28))->setName("Red mushroom");
Block* Block::BROWN_MUSHROOM = (new BlockPlant(17, 29))->setName("Brown mushroom");
Block* Block::OAK_PLANKS = (new Block(18, 4))->setName("Oak planks");
Block* Block::BIRCH_PLANKS = (new Block(19, 214))->setName("Birch planks");
Block* Block::PINE_PLANKS = (new Block(20, 198))->setName("Pine planks");
Block* Block::GRAVEL = (new Block(21, 19))->setName("Gravel");
Block* Block::COBBLESTONE = (new Block(22, 16))->setName("Cobblestone");
Block* Block::MOSSY_COBBLESTONE = (new Block(23, 36))->setName("Mossy cobblestone");
Block* Block::OBSIDIAN = (new Block(24, 37))->setName("Obsidian");
Block* Block::COB_WEB = (new BlockPlant(25, 11))->setName("Cobweb");
Block* Block::GLASS = (new BlockLeaves(26, 49))->setName("Glass");
Block* Block::BOOKSHELF = (new BlockLog(27, 35, 4))->setName("Bookshelf");

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
