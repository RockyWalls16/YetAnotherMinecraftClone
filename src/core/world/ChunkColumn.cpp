#include <core/world/ChunkColumn.h>
#include <util/Logger.h>
#include <string>

using namespace std;

void ChunkColumn::tick()
{
	for (auto it : chunks)
	{
		it.second->tick();
	}
}

shared_ptr<AirChunk> ChunkColumn::getChunkAt(int chunkY)
{
	auto it = chunks.find(chunkY);
	return it != chunks.end() ? it->second : nullptr;
}

void ChunkColumn::setChunkAt(shared_ptr<AirChunk> chunk, int chunkY)
{
	chunks[chunkY] = chunk;
}

void ChunkColumn::removeChunk(shared_ptr<AirChunk> chunk)
{
	chunks.erase(chunk->getChunkY());
}

bool ChunkColumn::isEmpty()
{
	return chunks.empty();
}

long long ChunkColumn::getColumnIndex(int chunkX, int chunkZ)
{
	// Generate Hashcode for column
	return (((long long)chunkX << (sizeof(int) * 8)) | (chunkZ & 0xffffffff));
}
