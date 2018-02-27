#ifndef CORE_WORLD_CHUNKGENERATORQUEUE_H_
#define CORE_WORLD_CHUNKGENERATORQUEUE_H_


#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <core/world/AirChunk.h>
#include <client/render/util/VertexBuilder.h>
#include <core/entity/Entity.h>

class ChunkGeneratorQueue
{
private:
	World & world;
	std::vector<weak_ptr<AirChunk>> chunkInputQueue;
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

	void pushInputChunk(const shared_ptr<AirChunk>& chunk);
	int getOutputSize() const;
	shared_ptr<AirChunk> popOutputChunk();

private:
	void onThreadStart();

	shared_ptr<AirChunk> popInputChunk();

	void pushOutputChunk(shared_ptr<AirChunk> chunk);
};

#endif