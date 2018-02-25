#ifndef CORE_WORLD_CHUNKCOLUMN_H_
#define CORE_WORLD_CHUNKCOLUMN_H_

#include <unordered_map>
#include <core/world/AirChunk.h>
#include <memory>
#include <util/Logger.h>

using namespace std;

class ChunkColumn
{
private:
	std::unordered_map<int, shared_ptr<AirChunk>> chunks;

public:
	~ChunkColumn();

	void tick();

	std::shared_ptr<AirChunk> getChunkAt(int chunkY);

	void setChunkAt(shared_ptr<AirChunk> chunk, int chunkY);

	void removeChunk(shared_ptr<AirChunk> chunk);

	bool isEmpty();

	static long long getColumnIndex(int chunkX, int chunkZ);
};

#endif