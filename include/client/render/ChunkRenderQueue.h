#ifndef CLIENT_RENDER_CHUNKRENDERQUEUE
#define CLIENT_RENDER_CHUNKRENDERQUEUE

#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <core/world/AirChunk.h>
#include <client/render/util/VertexBuilder.h>
#include <core/entity/Entity.h>

class ChunkRenderOutput
{
public:
	shared_ptr<AirChunk> chunk;
	VertexBuilder** vertexBuilders;

	ChunkRenderOutput(shared_ptr<AirChunk> ch, VertexBuilder** vb) : chunk(ch), vertexBuilders(vb) {}

	~ChunkRenderOutput()
	{
		delete(vertexBuilders[0]);
		delete(vertexBuilders[1]);
		delete(vertexBuilders);
	}
};

class ChunkRenderQueue
{
private:
	std::vector<shared_ptr<AirChunk>> chunkInputQueue;
	std::queue<ChunkRenderOutput*> chunkOutputQueue;

	//Thread management
	std::thread* renderQueueThread;
	std::condition_variable cv;
	std::mutex waitMutex;
	std::mutex inputMutex;
	std::mutex outputMutex;
	bool shallStop;

public:
	ChunkRenderQueue();
	~ChunkRenderQueue();

	void start();

	void pushInputChunk(shared_ptr<AirChunk> chunk);
	int getOutputSize();
	ChunkRenderOutput* popOutputChunk();

private:
	void onThreadStart();

	shared_ptr<AirChunk> popInputChunk();
	
	void pushOutputChunk(ChunkRenderOutput* chunkRenderOutput);
};

#endif