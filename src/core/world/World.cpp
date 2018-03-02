/*
 * World.cpp
 *
 *  Created on: 7 sept. 2017
 *      Author: Valentin
 */

#include <core/world/World.h>
#include <client/render/GameRenderer.h>
#include <core/entity/Entity.h>
#include <core/block/Block.h>
#include <util/Logger.h>
#include <util/TimeManager.h>
#include <core/world/ChunkGeneratorQueue.h>
#include <math/MathUtil.h>

World::World() : chunkGenerator(ChunkGenerator(*this)), chunkGeneratorQueue(*this)
{
	chunkGeneratorQueue.start();
}

void World::tick()
{
	time++;

	if (TimeManager::isMajorTick())
	{
		fetchReadyChunks();

		// Update chunks columns
		chunkManager.tickChunks();

		// Remove dead chunks
		for (shared_ptr<AirChunk> chunk : deadChunkQueue)
		{
			unloadChunk(chunk);
		}
		deadChunkQueue.clear();
	}

	// Update all entities
	for(Entity* entity : entityList)
	{
		entity->tick();
	}
}

void World::addEntity(Entity* entity)
{
	entityList.push_back(entity);
}

void World::keepAreaAlive(int x, int y, int z, int size)
{
	x = x >> CHUNK_SHIFT;
	y = y >> CHUNK_SHIFT;
	z = z >> CHUNK_SHIFT;

	int xLimit = x + size;
	int yLimit = y + size;
	int zLimit = z + size;

	ChunkLineZ lineZ;
	ChunkLineY lineY;
	for (int i = x - size; i < xLimit; i++)
	{
		lineZ = chunkManager.loadZLineFromX(i);
		for (int k = z - size; k < zLimit; k++)
		{
			lineY = chunkManager.loadYLineFromZ(lineZ, k);
			for (int j = y - size; j < yLimit; j++)
			{
				shared_ptr<AirChunk> chunk = chunkManager.getChunkFromY(lineY, j);
				if (chunk == nullptr)
				{
					// Generate new chunk & and add it to manager
					chunk = make_shared<AirChunk>(*this, i, j, k);
					chunkManager.setChunkAt(chunk, lineY);
					chunkGeneratorQueue.pushInputChunk(chunk);
				}

				chunk->resetTTL();
			}
		}
	}
}

shared_ptr<AirChunk> World::getChunkAt(int x, int y, int z)
{
	return chunkManager.getChunkAt(x, y, z);
}

void World::replaceChunkAt(shared_ptr<AirChunk> chunk)
{
	chunkManager.setChunkAt(chunk);
}

shared_ptr<AirChunk> World::getChunkAtBlockPos(int x, int y, int z)
{
	return getChunkAt(x >> CHUNK_SHIFT, y >> CHUNK_SHIFT, z >> CHUNK_SHIFT);
}

Block * World::getBlockAt(int x, int y, int z)
{
	shared_ptr<AirChunk> theChunk = getChunkAt(x >> CHUNK_SHIFT, y >> CHUNK_SHIFT, z >> CHUNK_SHIFT);
	if (!theChunk)
	{
		return Block::AIR;
	}

	return Block::getBlock(theChunk->getBlockAt(MathUtil::getChunkTilePosFromWorld(x), MathUtil::getChunkTilePosFromWorld(y), MathUtil::getChunkTilePosFromWorld(z)));
}

void World::setBlockAt(Block * block, int x, int y, int z, bool redrawChunk)
{
	shared_ptr<AirChunk> theChunk = getChunkAt(x >> CHUNK_SHIFT, y >> CHUNK_SHIFT, z >> CHUNK_SHIFT);
	if (!theChunk)
	{
		return;
	}

	theChunk->setBlockAt(block, MathUtil::getChunkTilePosFromWorld(x), MathUtil::getChunkTilePosFromWorld(y), MathUtil::getChunkTilePosFromWorld(z), redrawChunk);
}

void World::addChunkToUnload(const shared_ptr<AirChunk>& chunk)
{
	deadChunkQueue.push_back(chunk);
}

void World::unloadChunk(const shared_ptr<AirChunk>& chunk)
{
	// Get chunk column
	chunkManager.removeChunk(chunk);

	// Notify neighbour and delete chunk
	notifyNeighbours(chunk, NeighbourNotification::UNLOADED);
	onChunkUnReady(chunk);
}

void World::fetchReadyChunks()
{
	int chunkAmount = chunkGeneratorQueue.getOutputSize();
	for (int i = 0; i < chunkAmount; i++)
	{
		shared_ptr<AirChunk> chunk = chunkGeneratorQueue.popOutputChunk();
		
		chunk->setGenerated();
		chunkManager.setChunkAt(chunk);
		notifyNeighbours(chunk, NeighbourNotification::LOADED);
	}
}

void World::onChunkUnReady(const shared_ptr<AirChunk>& chunk)
{
	if (chunk->getChunkType() == ChunkType::LAYERED)
	{
		GameRenderer::getInstance().getWorldRenderer()->getChunkRenderer()->removeChunk(chunk);
	}
}

void World::onChunkDirty(const shared_ptr<AirChunk>& chunk)
{
	if (chunk->getChunkType() == ChunkType::LAYERED)
	{
		GameRenderer::getInstance().getWorldRenderer()->getChunkRenderer()->addChunkToRenderQueue(chunk);
	}
}

ChunkGenerator& World::getChunkGenerator()
{
	return chunkGenerator;
}

void World::notifyNeighbours(const shared_ptr<AirChunk>& chunk, NeighbourNotification type)
{
	// Update neighbours state
	int x = chunk->getChunkX();
	int y = chunk->getChunkY();
	int z = chunk->getChunkZ();

	notifySingleNeighbour(chunk, getChunkAt(x, y + 1, z), type, Side::BOTTOM);
	notifySingleNeighbour(chunk, getChunkAt(x, y - 1, z), type, Side::TOP);
	notifySingleNeighbour(chunk, getChunkAt(x + 1, y, z), type, Side::WEST);
	notifySingleNeighbour(chunk, getChunkAt(x - 1, y, z), type, Side::EAST);
	notifySingleNeighbour(chunk, getChunkAt(x, y, z + 1), type, Side::NORTH);
	notifySingleNeighbour(chunk, getChunkAt(x, y, z - 1), type, Side::SOUTH);
}

ChunkManager & World::getChunkManager()
{
	return chunkManager;
}

void World::notifySingleNeighbour(const shared_ptr<AirChunk>& sender, const shared_ptr<AirChunk>& chunk, NeighbourNotification type, Side fromSide)
{
	if (chunk != nullptr && chunk->isGenerated())
	{
		chunk->onNotifiedByNeighbour(type, sender, fromSide);
	}
}

long World::getTime()
{
	return time;
}
