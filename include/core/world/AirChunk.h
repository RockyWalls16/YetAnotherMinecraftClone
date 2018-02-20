/*
 * AirChunk.h
 *
 *  Created on: 13 sept. 2017
 *      Author: Valentin
 */

#ifndef CORE_WORLD_AIRCHUNK_H_
#define CORE_WORLD_AIRCHUNK_H_

#define CHUNK_SIZE 32
#define CHUNK_HALF 16
#define CHUNK_SHIFT 5

#define CHUNK_TTL 3

#include <util/Side.h>
#include <memory>
#include <util/Logger.h>

using namespace std;

class ChunkRenderIndex;
class World;

enum NeighbourNotification
{
	LOADED,
	UNLOADED,
	RESPONSE
};

enum ChunkType
{
	AIR,
	LAYERED
};

class AirChunk : public std::enable_shared_from_this<AirChunk>
{
protected:
	World* chunkWorld;
	int chunkX;
	int chunkY;
	int chunkZ;
	int neighbourCount;
	weak_ptr<AirChunk> neighbours[6];
	int timeToLive;

public:
	AirChunk(World* world, int chX, int chY, int chZ);
	virtual ~AirChunk(){}

	void tick();

	int getChunkX() const;
	int getChunkY() const;
	int getChunkZ() const;

	virtual short getBlockAt(int x, int y, int z);
	virtual void setBlockAt(short block, int x, int y, int z);
	void resetTTL();

	virtual ChunkType getChunkType();

	bool isReady();

	weak_ptr<AirChunk> getNeighbour(Side fromSide);

	void onNotifiedByNeighbour(NeighbourNotification loaded, shared_ptr<AirChunk> sender, Side fromSide);

	static int getFlatIndex(int x, int z);
};

#endif /* CORE_WORLD_AIRCHUNK_H_ */
