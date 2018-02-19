#ifndef _CORE_BLOCK_BLOCKGRASS_H_
#define _CORE_BLOCK_BLOCKGRASS_H_

#include <core/block/Block.h>

class BlockGrass : public Block
{
public:

	BlockGrass(unsigned short id) : Block(id, 8){};

	virtual int getTexture(Side faceSide) override
	{
		switch (faceSide)
		{
		case TOP: return 0;
		case BOTTOM: return 2;
		default: return 3;
		}
	}


};

#endif
