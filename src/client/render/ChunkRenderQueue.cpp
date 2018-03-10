#include "client/render/ChunkRenderQueue.h"
#include <util/Logger.h>
#include <client/render/GameRenderer.h>
#include <Game.h>
#include <limits>

ChunkRenderQueue::ChunkRenderQueue()
{
	shallStop = false;
	chunkInputQueue = vector<shared_ptr<AirChunk>>();
	chunkOutputQueue = queue<ChunkRenderOutput*>();
}

ChunkRenderQueue::~ChunkRenderQueue()
{
	Info("Stopping chunk render queue...");

	shallStop = true;

	// Notify thread for stop
	cv.notify_all();

	// Join not working with condition variable fix
	if (renderQueueThread->joinable())
	{
		renderQueueThread->join();
	}
}

void ChunkRenderQueue::start()
{
	renderQueueThread = new thread(&ChunkRenderQueue::onThreadStart, this);
}

void ChunkRenderQueue::onThreadStart()
{
	Info("Chunk Render Queue started !");

	unique_lock<mutex> waitLock(waitMutex);
	while (!shallStop)
	{
		// Prepare chunk mesh
		shared_ptr<AirChunk> inputChunk = popInputChunk();
		if(inputChunk != nullptr)
		{
			// Chunk is not ready anymore remove thread lock
			if (!inputChunk->isReady())
			{
				continue;
			}

			VertexBuilder** outputVb = GameRenderer::getInstance().getWorldRenderer()->getChunkRenderer().prepareChunkMesh(inputChunk);
			
			// Some chunks contains no vertices
			if (outputVb != nullptr)
			{
				pushOutputChunk(new ChunkRenderOutput(inputChunk, outputVb));
			}
		}
		else
		{
			cv.wait(waitLock);
		}
	}

	Info("Chunk Render Queue stopped !");
}

shared_ptr<AirChunk> ChunkRenderQueue::popInputChunk()
{
	// Lock input queue
	lock_guard<mutex> lock(inputMutex);
	// Check is not empty
	if (chunkInputQueue.size() == 0)
	{
		return nullptr;
	}

	// Pop nearest chunk to player
	float minDist = numeric_limits<float>::max();

	vector<shared_ptr<AirChunk>>::iterator nearestChunk;
	vector<shared_ptr<AirChunk>>::iterator it = chunkInputQueue.begin();
	glm::vec3 pos = Game::getInstance().getPlayer()->getPosition();
	while (it != chunkInputQueue.end())
	{
		// Check nearest chunk
		float x = (pos.x - (*it)->getChunkX() * CHUNK_SIZE + CHUNK_HALF);
		float y = (pos.y - (*it)->getChunkY() * CHUNK_SIZE + CHUNK_HALF);
		float z = (pos.z - (*it)->getChunkZ() * CHUNK_SIZE + CHUNK_HALF);
		float distance = x*x + y*y + z*z;

		if (distance < minDist)
		{
			nearestChunk = it;
			minDist = distance;
		}

		it++;
	}

	shared_ptr<AirChunk> inputChunk = *nearestChunk;
	chunkInputQueue.erase(nearestChunk);

	return inputChunk;
}

ChunkRenderOutput * ChunkRenderQueue::popOutputChunk()
{
	// Lock input queue
	lock_guard<mutex> lock(outputMutex);

	// Check is not empty
	if (chunkOutputQueue.size() == 0)
	{
		return nullptr;
	}

	// Pop input queue
	ChunkRenderOutput* outputChunk = chunkOutputQueue.front();
	chunkOutputQueue.pop();
	return outputChunk;
}

void ChunkRenderQueue::pushInputChunk(shared_ptr<AirChunk> chunk)
{
	// Lock input queue
	lock_guard<mutex> lock(inputMutex);
	chunkInputQueue.push_back(chunk);

	// Notify thread
	cv.notify_all();
}

int ChunkRenderQueue::getOutputSize()
{
	// Fast non synchronized method (executed every frame)
	return chunkOutputQueue.size();
}

void ChunkRenderQueue::pushOutputChunk(ChunkRenderOutput * chunkRenderOutput)
{
	// Lock output queue
	lock_guard<mutex> lock(outputMutex);
	chunkOutputQueue.push(chunkRenderOutput);
}


