#ifndef CORE_WORLD_CHUNKGENERATORQUEUE_H_
#define CORE_WORLD_CHUNKGENERATORQUEUE_H_


#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <core/world/AirChunk.h>
#include <client/render/util/VertexBuilder.h>
#include <core/entity/Entity.h>
#include <sparsepp/spp.h>

class ChunkGeneratorInput
{
public:
	int x;
	int y;
	int z;

	ChunkGeneratorInput(int x, int y, int z) : x(x), y(y), z(z) {}
};

class ChunkGeneratorQueue
{
private:
	World & world;
	std::vector<ChunkGeneratorInput*> chunkInputQueue;
	spp::sparse_hash_map<ChunkCoordKey, ChunkGeneratorInput*> workingChunkMap;
	std::queue<shared_ptr<AirChunk>> chunkOutputQueue;

	//Thread management
	std::thread* generatorQueueThread;
	std::condition_variable cv;
	std::mutex waitMutex;
	std::mutex inputMutex;
	std::mutex outputMutex;
	bool shallStop;

public:
	ChunkGeneratorQueue(World& world);
	~ChunkGeneratorQueue();

	void start();

	void pushInputChunk(int x, int y, int z, bool toDecorate);
	int getInputSize() const;
	int getOutputSize() const;
	shared_ptr<AirChunk> popOutputChunk();


	bool isGeneratorThread();

private:
	void onThreadStart();

	ChunkGeneratorInput* popInputChunk();

	void pushOutputChunk(shared_ptr<AirChunk> chunk);
};

#endif