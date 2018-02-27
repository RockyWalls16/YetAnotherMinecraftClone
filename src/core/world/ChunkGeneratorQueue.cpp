#include "core/world/ChunkGeneratorQueue.h"
#include <util/Logger.h>
#include <client/render/GameRenderer.h>
#include <Game.h>
#include <limits>

ChunkGeneratorQueue::ChunkGeneratorQueue(World& world) : world(world)
{
	shallStop = false;
	chunkInputQueue = vector<weak_ptr<AirChunk>>();
	chunkOutputQueue = queue<shared_ptr<AirChunk>>();
}

ChunkGeneratorQueue::~ChunkGeneratorQueue()
{
	Info("Stopping chunk generator queue...");

	shallStop = true;

	// Notify thread for stop
	cv.notify_all();

	// Join not working with condition variable fix
	if (generatorQueueThread->joinable())
	{
		generatorQueueThread->join();
	}
}

void ChunkGeneratorQueue::start()
{
	generatorQueueThread = new thread(&ChunkGeneratorQueue::onThreadStart, this);
}

void ChunkGeneratorQueue::onThreadStart()
{
	Info("Chunk Generator Queue started !");

	unique_lock<mutex> waitLock(waitMutex);
	while (!shallStop)
	{
		// Prepare chunk mesh
		shared_ptr<AirChunk> inputChunk = popInputChunk();
		if (inputChunk != nullptr)
		{
			shared_ptr<AirChunk> outputChunk = world.getChunkGenerator().generateChunk(inputChunk);
			pushOutputChunk(outputChunk);
		}
		else
		{
			cv.wait(waitLock);
		}
	}

	Info("Chunk Generator Queue stopped !");
}

shared_ptr<AirChunk> ChunkGeneratorQueue::popInputChunk()
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

	shared_ptr<AirChunk> nearestChunk;
	vector<weak_ptr<AirChunk>>::iterator itNearest;
	vector<weak_ptr<AirChunk>>::iterator it = chunkInputQueue.begin();
	glm::vec3 pos = Game::getInstance().getPlayer()->getPosition();
	while (it != chunkInputQueue.end())
	{
		// Check nearest chunk
		shared_ptr<AirChunk> chunk = it->lock();
		
		// Null chunk remove it (out of player area)
		if (!chunk)
		{
			it = chunkInputQueue.erase(it);
			continue;
		}

		float x = (pos.x - chunk->getChunkX() * CHUNK_SIZE + CHUNK_HALF);
		float y = (pos.y - chunk->getChunkY() * CHUNK_SIZE + CHUNK_HALF);
		float z = (pos.z - chunk->getChunkZ() * CHUNK_SIZE + CHUNK_HALF);
		float distance = x * x + y * y + z * z;

		if (distance < minDist)
		{
			itNearest = it;
			nearestChunk = chunk;
			minDist = distance;
		}

		it++;
	}

	chunkInputQueue.erase(itNearest);
	return nearestChunk;
}

shared_ptr<AirChunk> ChunkGeneratorQueue::popOutputChunk()
{
	// Lock input queue
	lock_guard<mutex> lock(outputMutex);

	// Check is not empty
	if (chunkOutputQueue.size() == 0)
	{
		return nullptr;
	}

	// Pop input queue
	shared_ptr<AirChunk> outputChunk = chunkOutputQueue.front();
	chunkOutputQueue.pop();
	return outputChunk;
}

void ChunkGeneratorQueue::pushInputChunk(const shared_ptr<AirChunk>& chunk)
{
	// Lock input queue
	lock_guard<mutex> lock(inputMutex);
	chunkInputQueue.push_back(chunk);

	// Notify thread
	cv.notify_all();
}

int ChunkGeneratorQueue::getOutputSize() const
{
	// Fast non synchronized method (executed every frame)
	return chunkOutputQueue.size();
}

void ChunkGeneratorQueue::pushOutputChunk(shared_ptr<AirChunk> chunk)
{
	// Lock output queue
	lock_guard<mutex> lock(outputMutex);
	chunkOutputQueue.push(chunk);
}


