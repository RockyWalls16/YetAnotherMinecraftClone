/*
 * WorldGen.cpp
 *
 *  Created on: 13 sept. 2017
 *      Author: Valentin
 */

#include <PerlinNoise.hpp>
#include <core/world/ChunkGenerator.h>
#include <core/world/AirChunk.h>
#include <core/world/ComplexChunk.h>
#include <util/Logger.h>
#include <string>

#define MIN_HEIGHT 20
#define NOISE_MULTIPLIER 100
#define MAX_HEIGHT (MIN_HEIGHT + NOISE_MULTIPLIER)

ChunkGenerator::ChunkGenerator(World& world) : world(world)
{
}

shared_ptr<AirChunk> ChunkGenerator::generateChunk(shared_ptr<AirChunk> inputChunk)
{
	static PerlinNoise perlin = PerlinNoise();

	int blockAmount = 0;
	int cX = inputChunk->getChunkX();
	int cY = inputChunk->getChunkY();
	int cZ = inputChunk->getChunkZ();

	// Check generation is not too high
	int rY = cY * CHUNK_SIZE;
	if (rY > MAX_HEIGHT)
	{
		// Above max height = empty chunk
		return inputChunk;
	}

	// Constant buffer
	short** layers = new short*[CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE; i++)
	{
		layers[i] = new short[CHUNK_SIZE * CHUNK_SIZE]{ 0 };
	}

	// Begin generation
	int rX = cX * CHUNK_SIZE;
	int rZ = cZ * CHUNK_SIZE;

	int layerIndex;
	int height;
	float noise;
	float noise2;
	int heightDiff;

	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int z = 0; z < CHUNK_SIZE; z++)
		{
			layerIndex = AirChunk::getFlatIndex(x, z);

			noise = perlin.octaveNoise0_1((rX + x) / 200.0F, (rZ + z) / 200.0F, 2);
			height = noise * NOISE_MULTIPLIER + MIN_HEIGHT;

			for (int y = 0; y < CHUNK_SIZE; y++)
			{
				int wY = rY + y;
				heightDiff = height - wY;

				if (wY < 46)
				{
					layers[y][layerIndex] = 5; // Water
				}

				if (wY < height)
				{
					if (heightDiff == 1)
					{
						layers[y][layerIndex] = 3; // Grass

						// Generate sand noise
						noise2 = perlin.octaveNoise((rX + x) / 50.0F, (rZ + z) / 50.0F);
						if (wY < 48 + noise2)
						{
							layers[y][layerIndex] = 4; // Sand
						}
					}
					else if (heightDiff < 5)
					{
						layers[y][layerIndex] = 2; // Dirt
					}
					else
					{
						layers[y][layerIndex] = 1; // Stone
					}
				}

				// If block generated
				if (layers[y][layerIndex] != 0)
				{
					blockAmount++;
				}
			}
		}
	}

	// Empty chunk
	if (blockAmount == 0)
	{
		return inputChunk;
	}
	else
	{
		// Contains blocs
		return make_shared<ComplexChunk>(world, cX, cY, cZ, layers);
	}
}
