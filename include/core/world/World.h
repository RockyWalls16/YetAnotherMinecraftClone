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
#include <core/world/ChunkGeneratorQueue.h>
#include <core/world/AirChunk.h>
#include <core/world/ChunkManager.h>
#include <unordered_map>
#include <memory>

using namespace std;

#define DAY_DURATION 1000

class Entity;
class EntityPlayer;
class Block;
class ChunkGeneratorQueue;

class World
{
private:
	ChunkManager chunkManager;
	ChunkGenerator chunkGenerator;
	ChunkGeneratorQueue generatorQueue;
	std::vector<Entity*> entityList;
	long time = 0;

public:
	World();

	void tick();
	void addEntity(Entity* entity);

	void keepAreaAlive(EntityPlayer& player, int x, int y, int z, int size);

	// Chunk management
	shared_ptr<AirChunk> getChunkAt(int x, int y, int z);
	shared_ptr<AirChunk> getChunkAtBlockPos(int x, int y, int z);
	void replaceChunkAt(shared_ptr<AirChunk> chunk);

	Block* getBlockAt(int x, int y, int z);
	void setBlockAt(Block* block, int x, int y, int z, bool redrawChunk = true);

	void fetchReadyChunks();
	void onChunkUnloaded(const shared_ptr<AirChunk>& chunk);
	void onChunkDirty(const shared_ptr<AirChunk>& chunk);

	ChunkGenerator& getChunkGenerator();

	long getTime();
	float World::getDayPercent();

	void notifyNeighbours(const shared_ptr<AirChunk>& chunk, NeighbourNotification type);

	ChunkManager& getChunkManager();
	ChunkGeneratorQueue& getChunkGeneratorQueue();

private:
	void notifySingleNeighbour(const shared_ptr<AirChunk>& sender, const shared_ptr<AirChunk>& chunk, NeighbourNotification type, Side fromSide);
};

#endif /* CORE_WORLD_WORLD_H_ */
