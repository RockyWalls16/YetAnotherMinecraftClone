/*
 * WorldGen.h
 *
 *  Created on: 13 sept. 2017
 *      Author: Valentin
 */

#ifndef CORE_WORLD_CHUNKGENERATOR_H_
#define CORE_WORLD_CHUNKGENERATOR_H_

#include <core/world/AirChunk.h>
#include <core/world/ComplexChunk.h>
#include <memory>

using namespace std;

class World;

class ChunkGenerator
{
private:
	World& world;

public:
	ChunkGenerator(World& world);

	shared_ptr<AirChunk> generateChunk(int cX, int cY, int cZ);

	void decorateChunk(shared_ptr<ComplexChunk> chunk);
};

#endif /* CORE_WORLD_CHUNKGENERATOR_H_ */
