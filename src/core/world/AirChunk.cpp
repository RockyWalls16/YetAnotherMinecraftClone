/*
 * AirChunk.cpp
 *
 *  Created on: 13 sept. 2017
 *      Author: Valentin
 */

#include <Game.h>
#include <core/world/AirChunk.h>
#include <client/render/ChunkRenderer.h>
#include <util/Logger.h>
#include <string>
#include <util/TimeManager.h>
#include <core/block/Block.h>
#include <core/world/ComplexChunk.h>

AirChunk::AirChunk(World& world, int chX, int chY, int chZ) : chunkWorld(world), chunkX(chX), chunkY(chY), chunkZ(chZ), neighbourCount(0), timeToLive(CHUNK_TTL), surfaceChunk(false), decorated(false)
{}

AirChunk::~AirChunk()
{

}

bool AirChunk::tick()
{
	timeToLive--;
	if (timeToLive == 0)
	{
		return false;
	}

	return true;
}

World& AirChunk::getWorld()
{
	return chunkWorld;
}

int AirChunk::getChunkX() const
{
	return chunkX;
}

int AirChunk::getChunkY() const
{
	return chunkY;
}

int AirChunk::getChunkZ() const
{
	return chunkZ;
}

short AirChunk::getBlockAt(int x, int y, int z)
{
	return 0;
}

void AirChunk::setBlockAt(Block* block, int x, int y, int z, bool reDraw)
{
	// Transform into ComplexChunk
	if (block != Block::AIR)
	{
		shared_ptr<ComplexChunk> complexChunk = make_shared<ComplexChunk>(chunkWorld, chunkX, chunkY, chunkZ);
		complexChunk->setBlockAt(block, x, y, z, false);
		complexChunk->setDecorated();
		complexChunk->chunkWorld.replaceChunkAt(complexChunk);
	}
}

void AirChunk::resetTTL()
{
	timeToLive = CHUNK_TTL;
}

ChunkType AirChunk::getChunkType()
{
	return ChunkType::AIR;
}

bool AirChunk::isRenderReady()
{
	return neighbourCount == 6 && decorated;
}

bool AirChunk::isDecorated()
{
	return decorated;
}

bool AirChunk::isSurfaceChunk()
{
	return surfaceChunk;
}

void AirChunk::setDecorated()
{
	decorated = true;

	if (isRenderReady())
	{
		chunkWorld.onChunkDirty(shared_from_this());
	}
}

void AirChunk::setSurfaceChunk()
{
	surfaceChunk = true;
}

weak_ptr<AirChunk> AirChunk::getNeighbour(Side fromSide)
{
	return neighbours[fromSide];
}

int AirChunk::getFlatIndex(int x, int y, int z)
{
	return y << (CHUNK_SHIFT * 2) | x << CHUNK_SHIFT | z;
}

void AirChunk::onNotifiedByNeighbour(NeighbourNotification loaded, shared_ptr<AirChunk> sender, Side fromSide)
{
	if (loaded != NeighbourNotification::UNLOADED)
	{
		if (!neighbours[fromSide].lock())
		{
			neighbourCount++;
		}

		neighbours[fromSide] = sender;

		// Notify spawned chunk of this chunk existence
		if (loaded != NeighbourNotification::RESPONSE)
		{
			sender->onNotifiedByNeighbour(NeighbourNotification::RESPONSE, shared_from_this(), SideUtil::opposite[fromSide]);
		}

		// Chunk is ready
		if (isRenderReady())
		{
			chunkWorld.onChunkDirty(shared_from_this());
		}
	}
	else
	{
		// Unload chunk
		if (isRenderReady())
		{
			chunkWorld.onChunkUnloaded(shared_from_this());
		}

		neighbourCount--;
		neighbours[fromSide].reset();
	}
}

void AirChunk::setDirty(Block* block, int x, int y, int z)
{
	refreshChunk();

	// Update neighbour chunks if block is on side
	if (block == Block::AIR)
	{
		shared_ptr<AirChunk> chunk = nullptr;
		if (x == 0 && (chunk = neighbours[Side::WEST].lock()))
		{
			chunk->refreshChunk();
		}
		else if (x == CHUNK_SIZE - 1 && (chunk = neighbours[Side::EAST].lock()))
		{
			chunk->refreshChunk();
		}

		if (y == 0 && (chunk = neighbours[Side::BOTTOM].lock()))
		{
			chunk->refreshChunk();
		}
		else if (y == CHUNK_SIZE - 1 && (chunk = neighbours[Side::TOP].lock()))
		{
			chunk->refreshChunk();
		}

		if (z == 0 && (chunk = neighbours[Side::NORTH].lock()))
		{
			chunk->refreshChunk();
		}
		else if (z == CHUNK_SIZE - 1 && (chunk = neighbours[Side::SOUTH].lock()))
		{
			chunk->refreshChunk();
		}
	}
}

void AirChunk::refreshChunk()
{
	if (isRenderReady())
	{
		chunkWorld.onChunkDirty(shared_from_this());
	}
}
