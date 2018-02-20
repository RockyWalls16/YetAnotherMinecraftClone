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

AirChunk::AirChunk(World* world, int chX, int chY, int chZ) : chunkWorld(world), chunkX(chX), chunkY(chY), chunkZ(chZ), neighbourCount(0), timeToLive(CHUNK_TTL)
{}

void AirChunk::tick()
{
	timeToLive--;
	if (timeToLive == 0)
	{
		chunkWorld->addChunkToUnload(shared_from_this());
	}
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

void AirChunk::setBlockAt(short block, int x, int y, int z)
{}

void AirChunk::resetTTL()
{
	timeToLive = CHUNK_TTL;
}

ChunkType AirChunk::getChunkType()
{
	return ChunkType::AIR;
}

bool AirChunk::isReady()
{
	return neighbourCount;
}

weak_ptr<AirChunk> AirChunk::getNeighbour(Side fromSide)
{
	return neighbours[fromSide];
}

int AirChunk::getFlatIndex(int x, int z)
{
	return x + (z << CHUNK_SHIFT);
}

void AirChunk::onNotifiedByNeighbour(NeighbourNotification loaded, shared_ptr<AirChunk> sender, Side fromSide)
{
	if (loaded == NeighbourNotification::LOADED || loaded == NeighbourNotification::RESPONSE)
	{
		neighbourCount++;
		neighbours[fromSide] = sender;

		// Notify spawned chunk of this chunk existence
		if (loaded == NeighbourNotification::LOADED)
		{
			sender->onNotifiedByNeighbour(NeighbourNotification::RESPONSE, shared_from_this(), SideUtil::opposite[fromSide]);
		}

		// Chunk is ready
		if (neighbourCount == 6)
		{
			chunkWorld->onChunkReady(shared_from_this());
		}
	}
	else if(loaded == NeighbourNotification::UNLOADED)
	{
		// Unload chunk
		if (neighbourCount == 6)
		{
			chunkWorld->onChunkUnReady(shared_from_this());
		}

		neighbourCount--;
		neighbours[fromSide].reset();
	}
}
