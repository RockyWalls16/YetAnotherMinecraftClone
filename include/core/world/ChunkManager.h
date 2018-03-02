#ifndef CORE_WORLD_CHUNKMANAGER_H_
#define CORE_WORLD_CHUNKMANAGER_H_

#include <core/world/AirChunk.h>
#include <unordered_map>
#include <memory>

using namespace std;
 
// Map for each axis
typedef shared_ptr<unordered_map<int, shared_ptr<AirChunk>>> ChunkLineY;
typedef shared_ptr<unordered_map<int, ChunkLineY>> ChunkLineZ;
typedef unordered_map<int, ChunkLineZ> ChunkLineX;

class ChunkManager
{
private:
	ChunkLineX chunkLines;

public:
	ChunkManager();

	shared_ptr<AirChunk> getChunkAt(int x, int y, int z);

	ChunkLineZ getZLineFromX(int x);

	ChunkLineY getYLineFromZ(const ChunkLineZ& lineZ, int z);

	shared_ptr<AirChunk> getChunkFromY(const ChunkLineY& lineY, int y);

	void tickChunks();

	ChunkLineZ loadZLineFromX(int x);

	ChunkLineY loadYLineFromZ(const ChunkLineZ& lineZ, int z);

	void setChunkAt(const shared_ptr<AirChunk>& chunk, const ChunkLineY& lineY);

	void setChunkAt(const shared_ptr<AirChunk>& chunk);

	void removeChunk(const shared_ptr<AirChunk>& chunk);
};

#endif