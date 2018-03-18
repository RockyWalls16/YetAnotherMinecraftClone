#ifndef CORE_WORLD_CHUNKMANAGER_H_
#define CORE_WORLD_CHUNKMANAGER_H_

#include <core/world/AirChunk.h>
#include <unordered_map>
#include <memory>
#include <vector>
#include <sparsepp/spp.h>

using namespace std;

class World;

class ChunkManager
{
private:
	World& world;
	spp::sparse_hash_map<ChunkCoordKey, shared_ptr<AirChunk>> chunkMap;
	spp::sparse_hash_map<ChunkCoordKey, shared_ptr<AirChunk>> loadingChunkMap;
	vector<shared_ptr<AirChunk>> loadedChunks;

public:
	ChunkManager(World& world);

	void tickChunks();

	shared_ptr<AirChunk> provideChunkAt(int x, int y, int z);

	shared_ptr<AirChunk> getChunkAt(int x, int y, int z);
	void insertChunkAt(shared_ptr<AirChunk> chunk);
	void setChunkAt(shared_ptr<AirChunk> chunk);

	void unloadChunk(const shared_ptr<AirChunk>& chunk);

	void clearLoadingChunkMap();

	shared_ptr<AirChunk> getLoadingChunkAt(int x, int y, int z);
};

#endif