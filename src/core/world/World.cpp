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
#include <math/MathUtil.h>

World::World() : chunkManager(*this), chunkGenerator(*this), generatorQueue(*this)
{
	generatorQueue.start();
	time = 3000;
}

void World::tick()
{
	time++;

	if (TimeManager::isMajorTick())
	{
		fetchReadyChunks();
		// Update chunks columns
		chunkManager.tickChunks();
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

	for (int i = x - size; i < xLimit; i++)
	{
		for (int k = z - size; k < zLimit; k++)
		{
			for (int j = y - size; j < yLimit; j++)
			{
				shared_ptr<AirChunk>& chunk = chunkManager.getChunkAt(i, j, k);

				// Chunk exists
				if(chunk)
				{
					if (!chunk->isDecorated())
					{
						generatorQueue.pushInputChunk(i, j, k, true);
					}

					chunk->resetTTL();
				}
				else
				{
					generatorQueue.pushInputChunk(i, j, k, false);
				}
			}
		}
	}
}

shared_ptr<AirChunk> World::getChunkAt(int x, int y, int z)
{
	return chunkManager.provideChunkAt(x, y, z);
}

shared_ptr<AirChunk> World::getChunkAtBlockPos(int x, int y, int z)
{
	return chunkManager.provideChunkAt(x >> CHUNK_SHIFT, y >> CHUNK_SHIFT, z >> CHUNK_SHIFT);
}

void World::replaceChunkAt(shared_ptr<AirChunk> chunk)
{
	chunkManager.setChunkAt(chunk);
	notifyNeighbours(chunk, NeighbourNotification::REPLACED);
}

Block * World::getBlockAt(int x, int y, int z)
{
	shared_ptr<AirChunk> theChunk = getChunkAtBlockPos(x, y, z);
	return Block::getBlock(theChunk->getBlockAt(x & (CHUNK_SIZE - 1), y & (CHUNK_SIZE - 1), z & (CHUNK_SIZE - 1)));
}

void World::setBlockAt(Block * block, int x, int y, int z, bool redrawChunk)
{
	shared_ptr<AirChunk> theChunk = getChunkAtBlockPos(x, y, z);
	theChunk->setBlockAt(block, x & (CHUNK_SIZE - 1), y & (CHUNK_SIZE - 1), z & (CHUNK_SIZE - 1), redrawChunk);
}

void World::fetchReadyChunks()
{
	int chunkAmount = generatorQueue.getOutputSize();
	for (int i = 0; i < chunkAmount; i++)
	{
		shared_ptr<AirChunk> chunk = generatorQueue.popOutputChunk();
		chunkManager.insertChunkAt(chunk);
		//notifyNeighbours(chunk, LOADED);
	}
}

void World::onChunkUnloaded(const shared_ptr<AirChunk>& chunk)
{
	if (chunk->getChunkType() == ChunkType::LAYERED)
	{
		GameRenderer::getInstance().getWorldRenderer()->getChunkRenderer().removeChunk(chunk);
	}
}

void World::onChunkDirty(const shared_ptr<AirChunk>& chunk)
{
	if (chunk->getChunkType() == ChunkType::LAYERED)
	{
		GameRenderer::getInstance().getWorldRenderer()->getChunkRenderer().addChunkToRenderQueue(chunk);
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

	notifySingleNeighbour(chunk, chunkManager.getChunkAt(x, y + 1, z), type, Side::BOTTOM);
	notifySingleNeighbour(chunk, chunkManager.getChunkAt(x, y - 1, z), type, Side::TOP);
	notifySingleNeighbour(chunk, chunkManager.getChunkAt(x + 1, y, z), type, Side::WEST);
	notifySingleNeighbour(chunk, chunkManager.getChunkAt(x - 1, y, z), type, Side::EAST);
	notifySingleNeighbour(chunk, chunkManager.getChunkAt(x, y, z + 1), type, Side::NORTH);
	notifySingleNeighbour(chunk, chunkManager.getChunkAt(x, y, z - 1), type, Side::SOUTH);
}

ChunkManager & World::getChunkManager()
{
	return chunkManager;
}

ChunkGeneratorQueue & World::getChunkGeneratorQueue()
{
	return generatorQueue;
}

void World::notifySingleNeighbour(const shared_ptr<AirChunk>& sender, const shared_ptr<AirChunk>& chunk, NeighbourNotification type, Side fromSide)
{
	if (chunk != nullptr && chunk->isDecorated())
	{
		chunk->onNotifiedByNeighbour(type, sender, fromSide);
	}
}

long World::getTime()
{
	return time;
}

float World::getDayPercent()
{
	float totalPercent = (time / (float)DAY_DURATION);
	return totalPercent - (int) totalPercent;
}
