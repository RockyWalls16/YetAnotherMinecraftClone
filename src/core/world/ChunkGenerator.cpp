/*
 * WorldGen.cpp
 *
 *  Created on: 13 sept. 2017
 *      Author: Valentin
 */

#include <FastNoiseSIMD/FastNoiseSIMD.h>
#include <core/world/ChunkGenerator.h>
#include <math/MathUtil.h>
#include <util/Logger.h>
#include <core/world/World.h>
#include <core/block/Block.h>
#include <string>

#define SURFACE 0
#define WATER_LEVEL 31
#define MAX_ELEVATION 120
#define MAX_PILLAR 50

ChunkGenerator::ChunkGenerator(World& world) : world(world)
{
}

shared_ptr<AirChunk> ChunkGenerator::generateChunk(int cX, int cY, int cZ)
{
	int blockAmount = 0;

	// Check generation is not too high
	int rY = cY * CHUNK_SIZE;
	if (rY > MAX_ELEVATION)
	{
		// Above max height = empty chunk
		return make_shared<AirChunk>(world, cX, cY, cZ);
	}

	// Begin generation
	shared_ptr<ComplexChunk> chunk = make_shared<ComplexChunk>(world, cX, cY, cZ);
	short* blocks = chunk->getData();

	// Check is underground chunk
	if (rY < SURFACE)
	{
		std::fill(blocks, blocks + CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE, 1); // Fill chunk with stone
		return chunk;
	}

	int rX = cX * CHUNK_SIZE;
	int rZ = cZ * CHUNK_SIZE;

	int layerIndex;
	float noise;
	int surfaceBlock = 0;

	static FastNoiseSIMD* elevationNoise = FastNoiseSIMD::NewFastNoiseSIMD();

	float* elevationNoiseSet = elevationNoise->GetPerlinFractalSet(rX, rY, rZ, CHUNK_SIZE, CHUNK_SIZE + 4, CHUNK_SIZE);
	//elevationNoise->FillPerlinFractalSet()
	int index = 0;

	for (int y = 0; y < CHUNK_SIZE + 4; y++)
	{
		float elevationDensityMultiplier = MathUtil::clamp(MathUtil::percent(SURFACE, MAX_ELEVATION, (float) rY + y), 0.0F, 1.0F);

		for (int x = 0; x < CHUNK_SIZE; x++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				noise = ((elevationNoiseSet[x * (CHUNK_SIZE + 4) * CHUNK_SIZE + y * CHUNK_SIZE + z] + 1.0F) * 0.5F);
				noise *= elevationDensityMultiplier * 0.2F + noise;

				// In chunk
				if (y < CHUNK_SIZE)
				{
					layerIndex = AirChunk::getFlatIndex(x, y, z);

					// Generate noise
					if (noise > elevationDensityMultiplier)
					{
						blocks[layerIndex] = 1; // Generate stone
					}
					else
					{
						if (rY + y < WATER_LEVEL)
						{
							blocks[layerIndex] = 5;
						}
					}

					// If block generated
					if (blocks[layerIndex] != 0)
					{
						blockAmount++;
					}
				}

				// Density is air
				if (noise <= elevationDensityMultiplier)
				{
					for (int depth = 1; depth <= 4; depth++)
					{
						int depthY = y - depth;
						if (depthY >= 0 && depthY < CHUNK_SIZE)
						{
							int depthLayerIndex = AirChunk::getFlatIndex(x, depthY, z);

							// Replace stone block by grass & dirt
							if (blocks[depthLayerIndex] == 1)
							{
								if (depth == 1)
								{
									surfaceBlock++;
									blocks[depthLayerIndex] = rY + y > WATER_LEVEL + 1 ? 3 : 4; // GRASS, SAND
								}
								else
								{
									blocks[depthLayerIndex] = rY + y > WATER_LEVEL + 1 ? 2 : 4; // DIRT, SAND
								}
							}
						}
					}
				}
			}
		}
	}

	if (surfaceBlock > 30)
	{
		chunk->setSurfaceChunk();
	}

	FastNoiseSIMD::FreeNoiseSet(elevationNoiseSet);

	// Empty chunk
	if (blockAmount == 0)
	{
		return make_shared<AirChunk>(world, cX, cY, cZ);
	}
	else
	{
		// Contains blocs
		return chunk;
	}
}

void ChunkGenerator::decorateChunk(shared_ptr<ComplexChunk> chunk)
{

	if (chunk->getChunkX() == 1 && chunk->getChunkY() == 1 && chunk->getChunkZ() == 0)
		Info("Dec " + std::to_string(chunk->getChunkX()) + " " + std::to_string(chunk->getChunkY()) + " " + std::to_string(chunk->getChunkZ()));


	if (chunk->isSurfaceChunk())
	{
		// Generate grass
		for (int i = 0, length = MathUtil::rand(50, 80); i < length; i++)
		{
			int x = MathUtil::rand(0, CHUNK_SIZE);
			int z = MathUtil::rand(0, CHUNK_SIZE);

			for (int y = 0; y < CHUNK_SIZE; y++)
			{
				if (chunk->getBlockAt(x, y, z) == 3)
				{
					world.setBlockAt(Block::TALL_GRASS, x + chunk->getChunkX() * CHUNK_SIZE, chunk->getChunkY() * CHUNK_SIZE + y + 1, z + chunk->getChunkZ() * CHUNK_SIZE);
				}
			}
		}

		// Generate flowers
		for (int i = 0, length = MathUtil::rand(0, 20); i < length; i++)
		{
			int x = MathUtil::rand(0, CHUNK_SIZE);
			int z = MathUtil::rand(0, CHUNK_SIZE);

			for (int y = 0; y < CHUNK_SIZE - 1; y++)
			{
				if (chunk->getBlockAt(x, y, z) == 3)
				{
					world.setBlockAt(MathUtil::randF() < 0.5F ? Block::FLOWER_ROSE : Block::FLOWER_DANDELION, x + chunk->getChunkX() * CHUNK_SIZE, chunk->getChunkY() * CHUNK_SIZE + y + 1, z + chunk->getChunkZ() * CHUNK_SIZE);
				}

				if (chunk->getBlockAt(x, y, z) == 4 && chunk->getBlockAt(x, y + 1, z) == 0) // SAND
				{
					for (int size = 0, length = MathUtil::rand(1, 3); size < length; size++)
					{
						world.setBlockAt(Block::SUGAR_CANE, x + chunk->getChunkX() * CHUNK_SIZE, chunk->getChunkY() * CHUNK_SIZE + y + size + 1, z + chunk->getChunkZ() * CHUNK_SIZE);
					}
				}
			}
		}

		// Generate trees
		for (int i = 0, length = MathUtil::rand(2, 6); i < length; i++)
		{
			int x = MathUtil::rand(0, CHUNK_SIZE);
			int z = MathUtil::rand(0, CHUNK_SIZE);

			for (int y = 0; y < CHUNK_SIZE; y++)
			{
				if (chunk->getBlockAt(x, y, z) == 3)
				{
					bool birchTree = MathUtil::randF() < 0.1F;
					for (int height = 0, treeHeight = MathUtil::rand(5, 7); height < treeHeight; height++)
					{
						int rY = chunk->getChunkY() * CHUNK_SIZE + y + 1 + height;

						// Generate potential mushroom
						if (height == 0 && MathUtil::randF() < 0.3F)
						{
							int mX = MathUtil::rand(-1, 2);
							int mZ = MathUtil::rand(-1, 2);
							if (world.getBlockAt(x + chunk->getChunkX() * CHUNK_SIZE + mX, rY, z + chunk->getChunkZ() * CHUNK_SIZE + mZ) == Block::AIR && world.getBlockAt(x + chunk->getChunkX() * CHUNK_SIZE + mX, rY - 1, z + chunk->getChunkZ() * CHUNK_SIZE + mZ) == Block::GRASS)
							{
								world.setBlockAt(MathUtil::randF() < 0.5F ? Block::BROWN_MUSHROOM : Block::RED_MUSHROOM, x + chunk->getChunkX() * CHUNK_SIZE + mX, rY, z + chunk->getChunkZ() * CHUNK_SIZE + mZ);
							}
						}

						// Generate trunk
						if (height <= treeHeight - 2)
						{
							world.setBlockAt(birchTree ? Block::WOOD_BIRCH_LOG : Block::WOOD_OAK_LOG, x + chunk->getChunkX() * CHUNK_SIZE, rY, z + chunk->getChunkZ() * CHUNK_SIZE);
						
						}
						// Generate leaves
						if (height >= treeHeight - 3)
						{
							int radius = height == treeHeight - 1 ? 1 : 2;
							for (int tX = x - radius; tX <= x + radius; tX++)
							{
								for (int tZ = z - radius; tZ <= z + radius; tZ++)
								{
									int rX = chunk->getChunkX() * CHUNK_SIZE + tX;
									int rZ = chunk->getChunkZ() * CHUNK_SIZE + tZ;
									if (world.getBlockAt(rX, rY, rZ) == Block::AIR)
									{
										world.setBlockAt(birchTree ? Block::LEAVES_BIRCH : Block::LEAVES_OAK, rX, rY, rZ);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
