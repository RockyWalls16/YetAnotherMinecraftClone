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

World::World()
{
	chunkGenerator = new ChunkGenerator(this);
	chunkGeneratorQueue = new ChunkGeneratorQueue();
	chunksColumns = std::unordered_map<long long, shared_ptr<ChunkColumn>>();
	chunkGeneratorQueue->start();
}

void World::tick()
{
	time++;

	if (TimeManager::isMajorTick())
	{
		fetchReadyChunks();

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
			shared_ptr<ChunkColumn> column = loadColumn(i, k);
			for (int j = y - size; j < yLimit; j++)
			{
				loadChunk(column, i, j, k)->resetTTL();
			}
		}
	}
}

shared_ptr<AirChunk> World::getChunkAt(int x, int y, int z)
{
	shared_ptr<ChunkColumn> cc = getColumnAt(x, z);
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

shared_ptr<ChunkColumn> World::getColumnAt(int chunkX, int chunkZ)
{
	// Search for chunk in map
	auto it = chunksColumns.find(ChunkColumn::getColumnIndex(chunkX, chunkZ));
	return it != chunksColumns.end() ? it->second : nullptr;
}

shared_ptr<ChunkColumn> World::loadColumn(int x, int z)
{
	// Check column already exists
	shared_ptr<ChunkColumn> column = getColumnAt(x, z);
	if (column != nullptr)
	{
		return column;
	}
	
	column = make_shared<ChunkColumn>();
	chunksColumns.insert(make_pair(ChunkColumn::getColumnIndex(x, z), column));

	return column;
}

shared_ptr<AirChunk> World::loadChunk(const shared_ptr<ChunkColumn>& column, int x, int y, int z)
{
	static int loadedChunk = 0;

	// Find chunk in column
	shared_ptr<AirChunk> chunk = column->getChunkAt(y);
	if (chunk != nullptr)
	{
		// Already exists no reason to load
		return chunk;
	}
	
	// Generate new chunk & and add it to column
	shared_ptr<AirChunk> outputChunk = make_shared<AirChunk>(this, x, y, z);
	column->setChunkAt(outputChunk, y);
	chunkGeneratorQueue->pushInputChunk(outputChunk);

	return outputChunk;
}

void World::addChunkToUnload(const shared_ptr<AirChunk>& chunk)
{
	deadChunkQueue.push_back(chunk);
}

void World::unloadChunk(const shared_ptr<AirChunk>& chunk)
{
	// Get chunk column
	shared_ptr<ChunkColumn> column = getColumnAt(chunk->getChunkX(), chunk->getChunkZ());
	column->removeChunk(chunk);

	// If column is empty remove it
	if (column->isEmpty())
	{
		chunksColumns.erase(ChunkColumn::getColumnIndex(chunk->getChunkX(), chunk->getChunkZ()));
		column.reset();
	}

	// Notify neighbour and delete chunk
	notifyNeighbours(chunk, NeighbourNotification::UNLOADED);
	onChunkUnReady(chunk);
}

void World::fetchReadyChunks()
{
	int chunkAmount = chunkGeneratorQueue->getOutputSize();
	for (int i = 0; i < chunkAmount; i++)
	{
		shared_ptr<AirChunk> chunk = chunkGeneratorQueue->popOutputChunk();
		shared_ptr<ChunkColumn> column = getColumnAt(chunk->getChunkX(), chunk->getChunkZ());
		if (column)
		{
			chunk->setGenerated();
			column->setChunkAt(chunk, chunk->getChunkY());
			notifyNeighbours(chunk, NeighbourNotification::LOADED);
		}
	}
}

void World::onChunkUnReady(const shared_ptr<AirChunk>& chunk)
{
	if (chunk->getChunkType() == ChunkType::LAYERED)
	{
		GameRenderer::getInstance().getWorldRenderer()->getChunkRenderer()->removeChunk(chunk);
	}
}

void World::onChunkReady(const shared_ptr<AirChunk>& chunk)
{
	if (chunk->getChunkType() == ChunkType::LAYERED)
	{
		GameRenderer::getInstance().getWorldRenderer()->getChunkRenderer()->addChunkToRenderQueue(chunk);
	}
}

ChunkGenerator * World::getChunkGenerator()
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
