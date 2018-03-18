#include <core/world/ChunkManager.h>
#include <core/world/World.h>

ChunkManager::ChunkManager(World& world) : world(world)
{
	chunkMap.reserve(4096);
	loadingChunkMap.reserve(1024);
}

shared_ptr<AirChunk> ChunkManager::provideChunkAt(int x, int y, int z)
{
	ChunkCoordKey key = ChunkCoordKey(x, y, z);

	auto it = chunkMap.find(key);

	// Chunk already exists
	if (it != chunkMap.end())
	{
		return it->second;
	}

	bool isGeneratorThread = world.getChunkGeneratorQueue().isGeneratorThread();

	// Fetch in second map
	if (isGeneratorThread)
	{
		auto it = loadingChunkMap.find(key);

		// Chunk already exists
		if (it != loadingChunkMap.end())
		{
			return it->second;
		}
	}

	// Chunk does not exists
	shared_ptr<AirChunk> newChunk = world.getChunkGenerator().generateChunk(x, y, z);
	if (newChunk->getChunkType() == AIR)
	{
		newChunk->setDecorated();
	}

	// Is from main thread ?
	if (!isGeneratorThread)
	{
		chunkMap[key] = newChunk;
		loadedChunks.push_back(newChunk);

		if (newChunk->isDecorated())
		{
			world.notifyNeighbours(newChunk, NeighbourNotification::LOADED);
		}
	}
	else
	{
		loadingChunkMap[key] = newChunk;
	}

	return newChunk;
}

shared_ptr<AirChunk> ChunkManager::getChunkAt(int x, int y, int z)
{
	auto it = chunkMap.find(ChunkCoordKey(x, y, z));
	return it != chunkMap.end() ? it->second : nullptr;
}

void ChunkManager::insertChunkAt(shared_ptr<AirChunk> chunk)
{
	int x = chunk->getChunkX();
	int y = chunk->getChunkY();
	int z = chunk->getChunkZ();

	ChunkCoordKey key(x, y, z);

	if (!chunkMap.contains(key))
	{
		chunkMap[key] = chunk;
		loadedChunks.push_back(chunk);
	}

	if (chunk->isDecorated())
	{
		world.notifyNeighbours(chunk, NeighbourNotification::LOADED);
	}
}

void ChunkManager::setChunkAt(shared_ptr<AirChunk> chunk)
{
	// TODO REPLACE IN LOADED CHUNKS VECTOR
	chunkMap[ChunkCoordKey(chunk->getChunkX(), chunk->getChunkY(), chunk->getChunkZ())] = chunk;
}

void ChunkManager::tickChunks()
{
	std::vector<shared_ptr<AirChunk>>::iterator it;
	for (it = loadedChunks.begin(); it != loadedChunks.end();)
	{
		shared_ptr<AirChunk>& chunk = (*it);

		// Shall remove chunk ?
		if (!chunk->tick())
		{
			unloadChunk(chunk);
			it = loadedChunks.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void ChunkManager::unloadChunk(const shared_ptr<AirChunk>& chunk)
{
	int x = chunk->getChunkX();
	int y = chunk->getChunkY();
	int z = chunk->getChunkZ();

	chunkMap.erase(ChunkCoordKey(x, y, z));

	world.notifyNeighbours(chunk, NeighbourNotification::UNLOADED);
	world.onChunkUnloaded(chunk);
}

void ChunkManager::clearLoadingChunkMap()
{
	loadingChunkMap.clear();
}

shared_ptr<AirChunk> ChunkManager::getLoadingChunkAt(int x, int y, int z)
{
	auto it = loadingChunkMap.find(ChunkCoordKey(x, y, z));
	return it != loadingChunkMap.end() ? it->second : nullptr;
}