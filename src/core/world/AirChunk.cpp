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

AirChunk::AirChunk(World* world, int chX, int chY, int chZ) : chunkWorld(world), chunkX(chX), chunkY(chY), chunkZ(chZ), neighbourCount(0), timeToLive(CHUNK_TTL), ready(false)
{
}

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

short AirChunk::getBlockAtWithNeighbours(Side side, int x, int y, int z)
{
	if (x >= 0 && y >= 0 && z >= 0 && x < CHUNK_SIZE && z < CHUNK_SIZE && y < CHUNK_SIZE)
	{
		return getBlockAt(x, y, z);
	}

	shared_ptr<AirChunk> other = neighbours[side].lock();
	if (other)
	{
		switch (side)
		{
		case TOP: return other->getBlockAt(x, 0, z); //Y+
		case BOTTOM: return other->getBlockAt(x, CHUNK_SIZE - 1, z); //Y-
		case EAST: return other->getBlockAt(0, y, z);  // X+
		case WEST: return other->getBlockAt(CHUNK_SIZE - 1, y, z); // X-
		case SOUTH: return other->getBlockAt(x, y, 0); // Z+
		case NORTH: return other->getBlockAt(x, y, CHUNK_SIZE - 1);  // Z-
		}
	}

	//Warning("Trying to mesh a not ready chunk !");
	return 0;
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
	return ready;
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

		if (neighbourCount == 6)
		{
			ready = true;
			chunkWorld->onChunkReady(shared_from_this());
		}

		// Notify spawned chunk of this chunk existence
		if (loaded == NeighbourNotification::LOADED)
		{
			sender->onNotifiedByNeighbour(NeighbourNotification::RESPONSE, shared_from_this(), SideUtil::opposite[fromSide]);
		}
	}
	else if(loaded == NeighbourNotification::UNLOADED)
	{
		neighbourCount--;
		neighbours[fromSide].reset();
		
		// Unload chunk
		if (ready)
		{
			chunkWorld->onChunkUnReady(shared_from_this());
		}

		ready = false;
	}
}
