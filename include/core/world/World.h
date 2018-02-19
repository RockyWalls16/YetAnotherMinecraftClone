/*
 * World.h
 *
 *  Created on: 7 sept. 2017
 *      Author: Valentin
 */

#ifndef CORE_WORLD_WORLD_H_
#define CORE_WORLD_WORLD_H_

#include <vector>
#include <core/world/ChunkGenerator.h>
#include <core/world/AirChunk.h>
#include <core/world/ChunkColumn.h>
#include <unordered_map>
#include <memory>

using namespace std;

#define DAY_DURATION 1000

class Entity;
class Block;

class World
{
private:
	std::unordered_map<long long, ChunkColumn*> chunksColumns;
	ChunkGenerator* chunkGenerator;
	std::vector<shared_ptr<AirChunk>> deadChunkQueue;
	std::vector<Entity*> entityList;
	long time;

public:
	World();

	void tick();
	void addEntity(Entity* entity);

	void keepAreaAlive(int x, int y, int z, int size);

	shared_ptr<AirChunk> getChunkAt(int x, int y, int z);
	shared_ptr<AirChunk> getChunkAtBlockPos(int x, int y, int z);

	Block* getBlockAt(int x, int y, int z);
	void setBlockAt(Block* block, int x, int y, int z);

	int getChunkTilePosFromWorld(int pos);

	shared_ptr<AirChunk> loadChunk(int x, int y, int z);
	void addChunkToUnload(shared_ptr<AirChunk> chunk);

	void onChunkUnReady(shared_ptr<AirChunk> chunk);
	void onChunkReady(shared_ptr<AirChunk> chunk);

	long getTime();

private:
	ChunkColumn* getColumnAt(int chunkX, int chunkZ);

	void notifyNeighbours(shared_ptr<AirChunk> chunk, NeighbourNotification type);

	void notifySingleNeighbour(shared_ptr<AirChunk> sender, shared_ptr<AirChunk> chunk, NeighbourNotification type, Side fromSide);

	void unloadChunk(shared_ptr<AirChunk> chunk);

};

#endif /* CORE_WORLD_WORLD_H_ */
