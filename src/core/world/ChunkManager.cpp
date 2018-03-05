#include <core/world/ChunkManager.h>

ChunkManager::ChunkManager()
{
	
}

shared_ptr<AirChunk> ChunkManager::getChunkAt(int x, int y, int z)
{
	ChunkLineZ lineZ = getZLineFromX(x);
	if (lineZ)
	{
		ChunkLineY lineY = getYLineFromZ(lineZ, z);
		if (lineY)
		{
			return getChunkFromY(lineY, y);
		}
	}

	return nullptr;
}

ChunkLineZ ChunkManager::getZLineFromX(int x)
{
	auto it = chunkLines.find(x);
	return it != chunkLines.end() ? it->second : nullptr;
}

ChunkLineY ChunkManager::getYLineFromZ(const ChunkLineZ& lineZ, int z)
{
	auto it = lineZ->find(z);
	return it != lineZ->end() ? it->second : nullptr;
}

shared_ptr<AirChunk> ChunkManager::getChunkFromY(const ChunkLineY& lineY, int y)
{
	auto it = lineY->find(y);
	return it != lineY->end() ? it->second : nullptr;
}

void ChunkManager::tickChunks()
{
	for (int length = loadedChunks.size(), i = length - 1; i >= 0; i--)
	{
		shared_ptr<AirChunk>& chunk = loadedChunks[i];

		// Shall remove chunk ?
		if (!chunk->tick())
		{
			loadedChunks.erase(loadedChunks.begin() + i);
		}
	}
}

ChunkLineZ ChunkManager::loadZLineFromX(int x)
{
	ChunkLineZ lineZ = getZLineFromX(x);
	if (lineZ)
	{
		return lineZ;
	}

	lineZ = make_shared<spp::sparse_hash_map<int, ChunkLineY>>();
	chunkLines[x] = lineZ;

	return lineZ;
}

ChunkLineY ChunkManager::loadYLineFromZ(const ChunkLineZ& lineZ, int z)
{
	ChunkLineY lineY = getYLineFromZ(lineZ, z);
	if (lineY)
	{
		return lineY;
	}

	lineY = make_shared<spp::sparse_hash_map<int, shared_ptr<AirChunk>>>();
	(*lineZ)[z] = lineY;

	return lineY;
}

shared_ptr<AirChunk> ChunkManager::getChunkFromNeighbour(const shared_ptr<AirChunk>& chunk, Side side)
{
	return chunk->getNeighbour(side).lock();
}

void ChunkManager::setChunkAt(const shared_ptr<AirChunk>& chunk, ChunkLineY& lineY)
{
	(*lineY)[chunk->getChunkY()] = chunk;
}

void ChunkManager::setChunkAt(const shared_ptr<AirChunk>& chunk)
{
	int x = chunk->getChunkX();
	int y = chunk->getChunkY();
	int z = chunk->getChunkZ();

	ChunkLineZ lineZ = getZLineFromX(x);
	if (lineZ)
	{

		ChunkLineY lineY = getYLineFromZ(lineZ, z);
		if (lineY)
		{
			setChunkAt(chunk, lineY);
			loadedChunks.push_back(chunk);
		}
	}
}

void ChunkManager::removeChunk(const shared_ptr<AirChunk>& chunk)
{
	int x = chunk->getChunkX();
	int y = chunk->getChunkY();
	int z = chunk->getChunkZ();

	ChunkLineZ lineZ = getZLineFromX(x);
	ChunkLineY lineY = getYLineFromZ(lineZ, z);

	lineY->erase(y);
	if (lineY->empty())
	{
		lineZ->erase(z);
		if (lineZ->empty())
		{
			chunkLines.erase(x);
		}
	}
}
