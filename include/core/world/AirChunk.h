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
#include <sparsepp/spp_utils.h>

using namespace std;

class ChunkRenderIndex;
class World;
class Block;

enum NeighbourNotification
{
	LOADED,
	UNLOADED,
	RESPONSE,
	REPLACED
};

enum ChunkType
{
	AIR,
	LAYERED
};

class AirChunk : public std::enable_shared_from_this<AirChunk>
{
protected:
	World& chunkWorld;
	int chunkX;
	int chunkY;
	int chunkZ;
	int neighbourCount;
	weak_ptr<AirChunk> neighbours[6];
	int timeToLive;
	bool decorated;
	bool surfaceChunk;

public:
	AirChunk(World& world, int chX, int chY, int chZ);
	virtual ~AirChunk();

	bool tick();

	World& getWorld();
	int getChunkX() const;
	int getChunkY() const;
	int getChunkZ() const;

	virtual short getBlockAt(int x, int y, int z);
	virtual void setBlockAt(Block* block, int x, int y, int z, bool reDraw);
	void resetTTL();

	virtual ChunkType getChunkType();

	bool isRenderReady();
	bool isDecorated();
	bool isSurfaceChunk();
	void setDecorated();
	void setSurfaceChunk();

	weak_ptr<AirChunk> getNeighbour(Side fromSide);
	void onNotifiedByNeighbour(NeighbourNotification loaded, shared_ptr<AirChunk> sender, Side fromSide);

	void setDirty(Block* block, int x, int y, int z);

	void refreshChunk();

	static int getFlatIndex(int x, int y, int z);
};

class ChunkCoordKey
{
public:
	int x;
	int y;
	int z;

	ChunkCoordKey(int x, int y, int z) : x(x), y(y), z(z) {}

	bool operator==(const ChunkCoordKey &other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}
};


namespace std
{
	// inject specialization of std::hash for Person into namespace std
	// ----------------------------------------------------------------
	template<>
	struct hash<ChunkCoordKey>
	{
		std::size_t operator()(ChunkCoordKey const &p) const
		{
			std::size_t seed = 0;
			spp::hash_combine(seed, p.x);
			spp::hash_combine(seed, p.y);
			spp::hash_combine(seed, p.z);
			return seed;
		}
	};
}


#endif /* CORE_WORLD_AIRCHUNK_H_ */
