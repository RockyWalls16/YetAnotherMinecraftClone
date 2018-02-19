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


World::World()
{
	chunkGenerator = new ChunkGenerator();
	chunksColumns = std::unordered_map<long long, ChunkColumn*>();
}

void World::tick()
{
	time++;

	// Update chunks columns
	for (auto it : chunksColumns)
	{
		it.second->tick();
	}

	// Remove dead chunks
	for (shared_ptr<AirChunk> chunk : deadChunkQueue)
	{
		unloadChunk(chunk);
	}
	deadChunkQueue.clear();

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

	for (int i = x - size; i < x + size; i++)
	{
		for (int k = z - size; k < z + size; k++)
		{
			for (int j = y - size; j < y + size; j++)
			{
				loadChunk(i, j, k)->resetTTL();
			}
		}
	}
}

shared_ptr<AirChunk> World::getChunkAt(int x, int y, int z)
{
	ChunkColumn* cc = getColumnAt(x, z);
	if (cc == nullptr)
	{
		return nullptr;
	}

	return cc->getChunkAt(y);
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

	return Block::getBlock(theChunk->getBlockAt(getChunkTilePosFromWorld(x), getChunkTilePosFromWorld(y), getChunkTilePosFromWorld(z)));
}

void World::setBlockAt(Block * block, int x, int y, int z)
{
	shared_ptr<AirChunk> theChunk = getChunkAt(x >> CHUNK_SHIFT, y >> CHUNK_SHIFT, z >> CHUNK_SHIFT);
	if (!theChunk)
	{
		return;
	}

	theChunk->setBlockAt(block->getId(), getChunkTilePosFromWorld(x), getChunkTilePosFromWorld(y), getChunkTilePosFromWorld(z));
}

int World::getChunkTilePosFromWorld(int pos)
{
	if (pos >= 0)
	{
		return pos % CHUNK_SIZE;
	}
	else
	{
		int i = pos % CHUNK_SIZE;
		return i != 0 ? i + CHUNK_SIZE : 0;
	}
}

ChunkColumn * World::getColumnAt(int chunkX, int chunkZ)
{
	// Search for chunk in map
	auto it = chunksColumns.find(ChunkColumn::getColumnIndex(chunkX, chunkZ));
	return it != chunksColumns.end() ? it->second : nullptr;
}

shared_ptr<AirChunk> World::loadChunk(int x, int y, int z)
{
	// Find column
	ChunkColumn* column = getColumnAt(x, z);
	if (column == nullptr)
	{
		// Create column if null
		chunksColumns[ChunkColumn::getColumnIndex(x, z)] = column = new ChunkColumn();
	}

	// Find chunk in column
	shared_ptr<AirChunk> chunk = column->getChunkAt(y);
	if (chunk != nullptr)
	{
		// Already exists no reason to load
		return chunk;
	}
	
	// Generate new chunk & and add it to column
	chunk = shared_ptr<AirChunk>(chunkGenerator->generateChunk(this, x, y, z));
	column->setChunkAt(chunk, y);

	notifyNeighbours(chunk, NeighbourNotification::LOADED);

	return chunk;
}

void World::addChunkToUnload(shared_ptr<AirChunk> chunk)
{
	deadChunkQueue.push_back(chunk);
}

void World::unloadChunk(shared_ptr<AirChunk> chunk)
{
	// Get chunk column
	ChunkColumn* column = getColumnAt(chunk->getChunkX(), chunk->getChunkZ());
	column->removeChunk(chunk);

	// If column is empty remove it
	if (column->isEmpty())
	{
		chunksColumns.erase(ChunkColumn::getColumnIndex(chunk->getChunkX(), chunk->getChunkZ()));
		delete(column);
	}

	// Notify neighbour and delete chunk
	notifyNeighbours(chunk, NeighbourNotification::UNLOADED);
	onChunkUnReady(chunk);
}

void World::onChunkUnReady(shared_ptr<AirChunk> chunk)
{
	if (chunk->getChunkType() == ChunkType::LAYERED)
	{
		GameRenderer::getInstance().getWorldRenderer()->getChunkRenderer()->removeChunk(chunk);
	}
}

void World::onChunkReady(shared_ptr<AirChunk> chunk)
{
	if (chunk->getChunkType() == ChunkType::LAYERED)
	{
		GameRenderer::getInstance().getWorldRenderer()->getChunkRenderer()->addChunkToRenderQueue(chunk);
	}
}

void World::notifyNeighbours(shared_ptr<AirChunk> chunk, NeighbourNotification type)
{
	// Update neighbours state
	notifySingleNeighbour(chunk, getChunkAt(chunk->getChunkX(), chunk->getChunkY() + 1, chunk->getChunkZ()), type, Side::BOTTOM);
	notifySingleNeighbour(chunk, getChunkAt(chunk->getChunkX(), chunk->getChunkY() - 1, chunk->getChunkZ()), type, Side::TOP);
	notifySingleNeighbour(chunk, getChunkAt(chunk->getChunkX() + 1, chunk->getChunkY(), chunk->getChunkZ()), type, Side::WEST);
	notifySingleNeighbour(chunk, getChunkAt(chunk->getChunkX() - 1, chunk->getChunkY(), chunk->getChunkZ()), type, Side::EAST);
	notifySingleNeighbour(chunk, getChunkAt(chunk->getChunkX(), chunk->getChunkY(), chunk->getChunkZ() + 1), type, Side::NORTH);
	notifySingleNeighbour(chunk, getChunkAt(chunk->getChunkX(), chunk->getChunkY(), chunk->getChunkZ() - 1), type, Side::SOUTH);
}

void World::notifySingleNeighbour(shared_ptr<AirChunk> sender, shared_ptr<AirChunk> chunk, NeighbourNotification type, Side fromSide)
{
	if (chunk != nullptr)
	{
		chunk->onNotifiedByNeighbour(type, sender, fromSide);
	}
}

long World::getTime()
{
	return time;
}
