/*
 * WorldGen.h
 *
 *  Created on: 13 sept. 2017
 *      Author: Valentin
 */

#ifndef CORE_WORLD_CHUNKGENERATOR_H_
#define CORE_WORLD_CHUNKGENERATOR_H_

#include <core/world/AirChunk.h>
#include <memory>

using namespace std;

class World;

class ChunkGenerator
{
public:
	shared_ptr<AirChunk> generateChunk(World* world, int x, int y, int z);
};

#endif /* CORE_WORLD_CHUNKGENERATOR_H_ */
