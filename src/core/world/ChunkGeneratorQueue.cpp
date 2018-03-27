#include "core/world/ChunkGeneratorQueue.h"
#include <util/Logger.h>
#include <util/GameSettings.h>
#include <client/render/GameRenderer.h>
#include <Game.h>
#include <limits>

ChunkGeneratorQueue::ChunkGeneratorQueue(World& world) : world(world)
{
	shallStop = false;
	workingChunkMap.reserve(4096);
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
		ChunkGeneratorInput* inputChunk = popInputChunk();
		if (inputChunk != nullptr)
		{
			bool shallSendAsOutput = false;

			// Not already generated ?
			shared_ptr<AirChunk> actualChunk = world.getChunkManager().getChunkAt(inputChunk->x, inputChunk->y, inputChunk->z);
			if (!actualChunk)
			{
				actualChunk = world.getChunkManager().getLoadingChunkAt(inputChunk->x, inputChunk->y, inputChunk->z);
				shallSendAsOutput = true;
			}

			if (!actualChunk)
			{
				shared_ptr<AirChunk> outputChunk = world.getChunkGenerator().generateChunk(inputChunk->x, inputChunk->y, inputChunk->z);

				// Is air chunk ?
				if (outputChunk->getChunkType() == AIR)
				{
					outputChunk->setDecorated();
				}

				pushOutputChunk(outputChunk);
			}
			else if(!actualChunk->isDecorated()) // Already generated check decorated
			{
				world.getChunkGenerator().decorateChunk(std::dynamic_pointer_cast<ComplexChunk>(actualChunk));
				actualChunk->setDecorated();

				if (true || shallSendAsOutput)
				{
					pushOutputChunk(actualChunk);
				}
			}

			delete(inputChunk);
		}
		else
		{
			world.getChunkManager().clearLoadingChunkMap();
			cv.wait(waitLock);
		}
	}

	Info("Chunk Generator Queue stopped !");
}

ChunkGeneratorInput* ChunkGeneratorQueue::popInputChunk()
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

	ChunkGeneratorInput* nearestChunk;
	vector<ChunkGeneratorInput*>::iterator itNearest;
	vector<ChunkGeneratorInput*>::iterator it = chunkInputQueue.begin();
	
	while (it != chunkInputQueue.end())
	{
		// Check nearest chunk
		ChunkGeneratorInput* chunk = *it;

		// Check chunk not out of player range
		shared_ptr<AirChunk>& playerChunk = chunk->player.getCurrentChunk();
		if (playerChunk)
		{
			if (abs(playerChunk->getChunkX() - chunk->x) > GameSettings::renderDistance || abs(playerChunk->getChunkZ() - chunk->z) > GameSettings::renderDistance || abs(playerChunk->getChunkY() - chunk->y) > GameSettings::renderDistance)
			{
				workingChunkMap.erase(ChunkCoordKey(chunk->x, chunk->y, chunk->z));
				it = chunkInputQueue.erase(it);
				delete(chunk);

				if (!chunkInputQueue.empty())
				{
					continue;
				}
				else
				{
					return nullptr;
				}
			}
		}


		glm::vec3 pos = chunk->player.getPosition();
		float x = (pos.x - chunk->x * CHUNK_SIZE + CHUNK_HALF);
		float y = (pos.y - chunk->y * CHUNK_SIZE + CHUNK_HALF);
		float z = (pos.z - chunk->z * CHUNK_SIZE + CHUNK_HALF);
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
	lock_guard<mutex> lock2(inputMutex);

	// Check is not empty
	if (chunkOutputQueue.size() == 0)
	{
		return nullptr;
	}

	// Pop input queue
	shared_ptr<AirChunk> outputChunk = chunkOutputQueue.front();
	workingChunkMap.erase(ChunkCoordKey(outputChunk->getChunkX(), outputChunk->getChunkY(), outputChunk->getChunkZ()));
	chunkOutputQueue.pop();

	return outputChunk;
}

void ChunkGeneratorQueue::pushInputChunk(EntityPlayer& player, int x, int y, int z)
{
	// Lock input queue
	lock_guard<mutex> lock(inputMutex);

	ChunkCoordKey key(x, y, z);
	if (!workingChunkMap.contains(key))
	{
		ChunkGeneratorInput* cgi = new ChunkGeneratorInput(player, x, y, z);
		chunkInputQueue.push_back(cgi);
		workingChunkMap[key] = cgi;

		// Notify thread
		cv.notify_all();
	}
}

int ChunkGeneratorQueue::getOutputSize() const
{
	// Fast non synchronized method (executed every frame)
	return chunkOutputQueue.size();
}

int ChunkGeneratorQueue::getInputSize() const
{
	// Fast non synchronized method (executed every frame)
	return chunkInputQueue.size();
}

void ChunkGeneratorQueue::pushOutputChunk(shared_ptr<AirChunk> chunk)
{
	// Lock output queue
	lock_guard<mutex> lock(outputMutex);
	chunkOutputQueue.push(chunk);
}

bool ChunkGeneratorQueue::isGeneratorThread()
{
	return std::this_thread::get_id() == generatorQueueThread->get_id();
}


