#ifndef _CORE_BLOCK_BLOCKAIR_H_
#define _CORE_BLOCK_BLOCKAIR_H_

#include <core/block/Block.h>
#include <math/AABB.h>

class BlockAir : public Block
{
public:

	BlockAir(unsigned short id) : Block(id, 0) {};

	virtual bool isVisible() override { return false; }

	virtual bool canCollide() override { return false; }
};

#endif
