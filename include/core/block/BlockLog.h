#ifndef _CORE_BLOCK_BLOCKLOG_H_
#define _CORE_BLOCK_BLOCKLOG_H_

#include <core/block/Block.h>

class BlockLog : public Block
{
private:
	int topTexture;

public:

	BlockLog(unsigned short id, int sideTexture, int topTexture) : Block(id, sideTexture), topTexture(topTexture){};

	virtual int getTexture(Side faceSide) override
	{
		switch (faceSide)
		{
		case TOP:
		case BOTTOM: return topTexture;
		default: return textureId;
		}
	}


};

#endif
