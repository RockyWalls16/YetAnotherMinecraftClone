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

AirChunk::AirChunk(World* world, int chX, int chY, int chZ) : chunkWorld(world), chunkX(chX), chunkY(chY), chunkZ(chZ), neighbourCount(0), timeToLive(CHUNK_TTL), generated(false)
{}

AirChunk::~AirChunk()
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

World * AirChunk::getWorld()
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
		short** layers = new short*[CHUNK_SIZE];
		for (int i = 0; i < CHUNK_SIZE; i++)
		{
			layers[i] = new short[CHUNK_SIZE * CHUNK_SIZE]{ 0 };
		}
		layers[y][getFlatIndex(x, z)] = block->getId();

		shared_ptr<ComplexChunk> complexChunk = make_shared<ComplexChunk>(chunkWorld, chunkX, chunkY, chunkZ, layers);
		complexChunk->setGenerated();
		complexChunk->chunkWorld->replaceChunkAt(complexChunk, chunkX, chunkY, chunkZ);
		complexChunk->chunkWorld->notifyNeighbours(complexChunk, NeighbourNotification::REPLACED);
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

bool AirChunk::isReady()
{
	return neighbourCount == 6;
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
	if (loaded != NeighbourNotification::UNLOADED)
	{
		if (loaded != NeighbourNotification::REPLACED)
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
		if (neighbourCount == 6 && loaded != NeighbourNotification::REPLACED)
		{
			chunkWorld->onChunkDirty(shared_from_this());
		}
	}
	else
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

bool AirChunk::isGenerated()
{
	return generated;
}

void AirChunk::setGenerated()
{
	generated = true;
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
	chunkWorld->onChunkDirty(shared_from_this());
}
