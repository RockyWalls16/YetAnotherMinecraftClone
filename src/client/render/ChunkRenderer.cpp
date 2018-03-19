/*
 * ChunkRenderer.cpp
 *
 *  Created on: 13 sept. 2017
 *      Author: Valentin
 */

#include <Game.h>
#include <core/world/World.h>
#include <core/world/AirChunk.h>
#include <client/render/ChunkRenderer.h>
#include <client/render/BlockRenderer.h>
#include <client/render/util/VertexBuilder.h>
#include <client/shaders/ShaderCache.h>
#include <client/textures/TextureCache.h>
#include <core/block/Block.h>
#include <util/Logger.h>
#include <string>
#include <glm/vec3.hpp>
#include <client/render/GameRenderer.h>
#include <client/render/ChunkRenderQueue.h>
#include <math/Frustum.h>
#include <algorithm>
#include <limits>

ChunkRenderer::ChunkRenderer() : chunkCount(0), chunkDrawed(0)
{
	chunkRenderQueue.start();
}

ChunkRenderer::~ChunkRenderer()
{}

void ChunkRenderer::render(RenderLayer renderLayer)
{
	if (renderLayer == RenderLayer::RL_OPAQUE)
	{
		chunkDrawed = 0;
	}

	if (renderLayer == RenderLayer::RL_OPAQUE)
	{
		ShaderCache::deferredBlockShader->use();
	}
	else
	{
		ShaderCache::forwardBlockShader->use();
	}
	TextureCache::blockTexture->bind();
	TextureCache::blockSpecularTexture->bind(1);

	fetchReadyChunks();

	// Render chunks opaque then transparent
	renderChunkLayer(renderLayer);

	TextureCache::blockTexture->unbind();
}

void ChunkRenderer::renderChunkLayer(RenderLayer renderLayer)
{
	Camera& camera = GameRenderer::getInstance().getGameCamera();
	float cameraY = camera.getLocation().y;
	ChunkRenderContainer& container = getColumnContainer(renderLayer);

	for (auto columnPair : container.columnsMap)
	{
		shared_ptr<ChunkRenderColumn>& column = columnPair.second;
	
		int chunkX = column->chunkX * CHUNK_SIZE;
		int chunkZ = column->chunkZ * CHUNK_SIZE;

		// Check column is in frustum
		if (Frustum::columnInFrustum(chunkX, (int) cameraY - CHUNK_SIZE * 10, chunkZ, CHUNK_SIZE, (int) cameraY + CHUNK_SIZE * 10, CHUNK_SIZE))
		{
			for (ChunkRenderIndex* cri : column->column)
			{
				shared_ptr<AirChunk>& chunk = cri->chunk;
				if (Frustum::boxInFrustum(chunk->getChunkX() * CHUNK_SIZE, chunk->getChunkY() * CHUNK_SIZE, chunk->getChunkZ() * CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE))
				{
					cri->mesh->drawEBO(cri->vertexAmount);
					chunkDrawed++;
				}
			}
		}
	}
}

void ChunkRenderer::fetchReadyChunks()
{
	// Check ChunkRenderQueue for ready chunk to add as VAO
	int chunkAmount = chunkRenderQueue.getOutputSize();
	for (int i = 0; i < chunkAmount; i++)
	{
		ChunkRenderOutput* cru = chunkRenderQueue.popOutputChunk();

		shared_ptr<ChunkRenderColumn> opaqueColumn = getRenderColumn(RenderLayer::RL_OPAQUE, cru->chunk->getChunkX(), cru->chunk->getChunkZ());
		shared_ptr<ChunkRenderColumn> transparentColumn = getRenderColumn(RenderLayer::RL_TRANSPARENT, cru->chunk->getChunkX(), cru->chunk->getChunkZ());
		addChunkToContainer(opaqueContainer, opaqueColumn, cru->chunk, cru->vertexBuilders[0], cru->vertexBuilders[0]->getIndicesWriteIndex());
		addChunkToContainer(transparentContainer, transparentColumn, cru->chunk, cru->vertexBuilders[1], cru->vertexBuilders[1]->getIndicesWriteIndex());

		delete(cru);
	}
}

void ChunkRenderer::addChunkToContainer(ChunkRenderContainer& columnContainer, shared_ptr<ChunkRenderColumn> layerColumn, shared_ptr<AirChunk>& chunk, VertexBuilder* builders, int vertexAmount)
{
	// Create new chunk render index
	ChunkRenderIndex* cri = nullptr;
	if (vertexAmount > 0)
	{
		VertexArray* vao = new VertexArray();
		configureVAO(chunk, builders, vao);
		cri = new ChunkRenderIndex(chunk, vao, builders->getIndicesWriteIndex());
	}

	// Check chunk if already renderer
	if (layerColumn != nullptr)
	{
		for (int i = 0, length = layerColumn->column.size(); i < length; i++)
		{
			ChunkRenderIndex* oldCri = layerColumn->column[i];
			if (oldCri->chunk == chunk)
			{
				// Replace oldCri by new cri and delete old one
				delete(oldCri);

				// Replace chunk
				if (cri)
				{
					layerColumn->column[i] = cri;
					return;
				}
				else
				{
					// Erase chunk if nothing new
					layerColumn->column.erase(layerColumn->column.begin() + i);
					chunkCount--;
					break;
				}
			}
		}
	}

	// Chunk not present add it to render list
	if (cri)
	{
		if (layerColumn == nullptr)
		{
			layerColumn = make_shared<ChunkRenderColumn>(chunk->getChunkX(), chunk->getChunkZ());
			columnContainer.columnsMap.insert(make_pair(getColumnIndex(chunk->getChunkX(), chunk->getChunkZ()), layerColumn));
		}

		layerColumn->column.push_back(cri);
		chunkCount++;
	}
}

void ChunkRenderer::addChunkToRenderQueue(const shared_ptr<AirChunk>& chunk)
{
	chunkRenderQueue.pushInputChunk(chunk);
}

VertexBuilder** ChunkRenderer::prepareChunkMesh(const shared_ptr<AirChunk>& chunk)
{
	// Add chunks to FBO, array index [0] opaque [1] transparent
	VertexBuilder** vertexBuilders = new VertexBuilder*[2]{ new VertexBuilder(11, 24000), new VertexBuilder(11, 6000) };

	applyStandardMesh(vertexBuilders, chunk);

	// No vertices to render
	if (vertexBuilders[0]->getEOBSize() == 0 && vertexBuilders[1]->getEOBSize() == 0)
	{
		// Clean memory
		delete(vertexBuilders[0]);
		delete(vertexBuilders[1]);
		delete(vertexBuilders);

		return nullptr;
	}

	return vertexBuilders;
}

/***
 *  Greedy meshing algorithm. 
 */
void ChunkRenderer::applyGreedyMeshing(VertexBuilder** builders, const shared_ptr<AirChunk>& ch)
{
	// Lock chunk neighbours
	shared_ptr<AirChunk> neighbours[6];
	for (int i = 0; i < 6; i++)
	{
		neighbours[i] = ch->getNeighbour((Side)i).lock();
		if (!neighbours[i])
		{
			return;
		}
	}

	bool mask[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE][6]{false};
	int width, height, l;
	int side;
	int x, y, z;
	int x2, y2, z2;

	// For each and every block in chunk
	for (y = 0; y < CHUNK_SIZE; y++)
	{
		for (x = 0; x < CHUNK_SIZE; x++)
		{
			for (z = 0; z < CHUNK_SIZE; z++)
			{
				Block* block = Block::getBlock(ch->getBlockAt(x, y, z));

				if (!block->isVisible()) // Check block is visible
				{
					for (side = 0; side < 6; side++)
					{
						mask[x][y][z][side] = true;
					}
					continue;
				}
				RenderType renderType = block->getRenderType();
				int renderLayer = block->getRenderLayer();

				if (renderType == RenderType::BLOCK) // Check block is visible
				{
					AABB blockAABB = block->getRenderHitbox();
					for (side = 0; side < 6; side++) // For eachs side
					{
						if (!mask[x][y][z][side] && BlockRenderer::isSideVisible(ch, neighbours, block, x, y, z, (Side)side)) // Check not already meshed and side is visible
						{
							if (side == Side::TOP || side == Side::BOTTOM) // Check for near top then bottom face
							{
								// Searh for identical neighbour faces
								for (x2 = x; x2 < CHUNK_SIZE; x2++)
								{
									if (x2 == x)
									{
										for (z2 = z; z2 < CHUNK_SIZE; z2++)
										{
											if (!mask[x2][y][z2][side] && ch->getBlockAt(x2, y, z2) == block->getId() && BlockRenderer::isSideVisible(ch, neighbours, block, x2, y, z2, (Side)side))
											{
												mask[x2][y][z2][side] = true;
											}
											else
											{
												break;
											}
										}
										width = z2 - z;
									}
									else
									{
										l = z + width;
										for (z2 = z; z2 < l; z2++)
										{
											if (mask[x2][y][z2][side] || ch->getBlockAt(x2, y, z2) != block->getId() || !BlockRenderer::isSideVisible(ch, neighbours, block, x2, y, z2, (Side)side))
											{
												goto mainLoopTB;
											}
										}

										// Mark new line as meshed
										for (z2 = z; z2 < l; z2++)
										{
											mask[x2][y][z2][side] = true;
										}
									}
								}
							mainLoopTB:

								BlockRenderer::renderFace(builders[renderLayer], block, blockAABB, x, y, z, x2 - x, 1, width, (Side)side);
							}
							else if (side == Side::EAST || side == Side::WEST)
							{
								// Searh for identical neighbour faces
								for (y2 = y; y2 < CHUNK_SIZE; y2++)
								{
									if (y2 == y)
									{
										for (z2 = z; z2 < CHUNK_SIZE; z2++)
										{
											if (!mask[x][y2][z2][side] && ch->getBlockAt(x, y2, z2) == block->getId() && BlockRenderer::isSideVisible(ch, neighbours, block, x, y2, z2, (Side)side))
											{
												mask[x][y2][z2][side] = true;

												// Block is not full height, no need to check for further Y neighbours
												if (blockAABB.startPos.y != 0.0 || blockAABB.endPos.y != 1.0)
												{
													z2++;
													break;
												}
											}
											else
											{
												break;
											}
										}
										width = z2 - z;
									}
									else
									{
										l = z + width;
										for (z2 = z; z2 < l; z2++)
										{
											if (mask[x][y2][z2][side] || ch->getBlockAt(x, y2, z2) != block->getId() || !BlockRenderer::isSideVisible(ch, neighbours, block, x, y2, z2, (Side)side))
											{
												goto mainLoopNS;
											}
										}

										// Mark new line as meshed
										for (z2 = z; z2 < l; z2++)
										{
											mask[x][y2][z2][side] = true;
										}
									}
								}
							mainLoopNS:

								BlockRenderer::renderFace(builders[renderLayer], block, blockAABB, x, y, z, 1, y2 - y, width, (Side)side);
							}
							else if (side == Side::NORTH || side == Side::SOUTH)
							{
								// Searh for identical neighbour faces
								for (x2 = x; x2 < CHUNK_SIZE; x2++)
								{
									if (x2 == x)
									{
										for (y2 = y; y2 < CHUNK_SIZE; y2++)
										{
											if (!mask[x2][y2][z][side] && ch->getBlockAt(x2, y2, z) == block->getId() && BlockRenderer::isSideVisible(ch, neighbours, block, x2, y2, z, (Side)side))
											{
												mask[x2][y2][z][side] = true;

												// Block is not full height, no need to check for further Y neighbours
												if (blockAABB.startPos.y != 0.0 || blockAABB.endPos.y != 1.0)
												{
													y2++;
													break;
												}
											}
											else
											{
												break;
											}
										}
										height = y2 - y;
									}
									else
									{
										l = y + height;
										for (y2 = y; y2 < l; y2++)
										{
											if (mask[x2][y2][z][side] || ch->getBlockAt(x2, y2, z) != block->getId() || !BlockRenderer::isSideVisible(ch, neighbours, block, x2, y2, z, (Side)side))
											{
												goto mainLoopWE;
											}
										}

										// Mark new line as meshed
										for (y2 = y; y2 < l; y2++)
										{
											mask[x2][y2][z][side] = true;
										}
									}
								}
							mainLoopWE:

								BlockRenderer::renderFace(builders[renderLayer], block, blockAABB, x, y, z, x2 - x, height, 1, (Side)side);
							}
						}
					}
				}
				else
				{
					// Mark all faces as masked
					for (side = 0; side < 6; side++)
					{
						mask[x][y][z][side] = true;
					}

					BlockRenderer::renderBlock(builders[renderLayer], block, renderType, x, y, z);
				}
			}
		}
	}
}

void ChunkRenderer::applyStandardMesh(VertexBuilder ** builders, const shared_ptr<AirChunk>& ch)
{
	int side;
	int x, y, z;

	shared_ptr<AirChunk> neighbours[6];
	for (int i = 0; i < 6; i++)
	{
		neighbours[i] = ch->getNeighbour((Side)i).lock();
		if (!neighbours[i])
		{
			return;
		}
	}

	for (y = 0; y < CHUNK_SIZE; y++)
	{
		for (x = 0; x < CHUNK_SIZE; x++)
		{
			for (z = 0; z < CHUNK_SIZE; z++)
			{
				Block* block = Block::getBlock(ch->getBlockAt(x, y, z));

				if (block->isVisible()) // Check block is visible
				{
					RenderType renderType = block->getRenderType();
					int renderLayer = block->getRenderLayer();

					if (renderType == RenderType::BLOCK) // Check block is visible
					{
						AABB blockAABB = block->getRenderHitbox();

						for (side = 0; side < 6; side++)
						{
							if (BlockRenderer::isSideVisible(ch, neighbours, block, x, y, z, (Side)side))
							{
								BlockRenderer::renderSimpleFace(builders[renderLayer], block, blockAABB, x, y, z, (Side)side);
							}
						}
					}
					else
					{
						BlockRenderer::renderBlock(builders[renderLayer], block, renderType, x, y, z);
					}
				}
			}
		}
	}
}

void ChunkRenderer::removeChunk(const shared_ptr<AirChunk>& chunk)
{
	removeChunkAtLayer(chunk, RL_OPAQUE);
	removeChunkAtLayer(chunk, RL_TRANSPARENT);
}

void ChunkRenderer::removeChunkAtLayer(const shared_ptr<AirChunk>& chunk, RenderLayer renderLayer)
{
	ChunkRenderContainer& columnContainer = getColumnContainer(renderLayer);
	shared_ptr<ChunkRenderColumn> column = getRenderColumn(renderLayer, chunk->getChunkX(), chunk->getChunkZ());
	if (column != nullptr)
	{
		// Iterate over column
		vector<ChunkRenderIndex*>::iterator iter;
		for (iter = column->column.begin(); iter != column->column.end(); iter++)
		{
			if ((*iter)->chunk == chunk)
			{
				ChunkRenderIndex* cri = *iter;

				delete(*iter);
				iter = column->column.erase(iter);
				chunkCount--;

				// Free transparent column memory
				if (column->column.empty())
				{
					columnContainer.columnsMap.erase(getColumnIndex(chunk->getChunkX(), chunk->getChunkZ()));
				}

				return;
			}
		}
	}
}

ChunkRenderContainer& ChunkRenderer::getColumnContainer(RenderLayer layer)
{
	if (layer == RenderLayer::RL_OPAQUE)
	{
		return opaqueContainer;
	}
	else
	{
		return transparentContainer;
	}
}

shared_ptr<ChunkRenderColumn> ChunkRenderer::getRenderColumn(RenderLayer layer, int x, int z)
{
	// Returns chunk column for layer and pos
	ChunkRenderContainer& column = getColumnContainer(layer);
	auto it = column.columnsMap.find(getColumnIndex(x, z));
	return it != column.columnsMap.end() ? it->second : nullptr;
}

void ChunkRenderer::configureVAO(const shared_ptr<AirChunk>& chunk, VertexBuilder * builder, VertexArray * vertexArray)
{
	vertexArray->addVBO(builder->getVertexBuffer(), builder->getVBOSize(), GL_STATIC_DRAW);
	vertexArray->enableEBO(builder->getIndicesBuffer(), builder->getEOBSize(), GL_STATIC_DRAW);
	vertexArray->assignPositionAttrib(0, 0, 11 * sizeof(float)); // Position
	vertexArray->assignRGBAttrib(0, 1, 11 * sizeof(float), (void*)(3 * sizeof(float))); // Colors
	vertexArray->assignPositionAttrib(0, 2, 11 * sizeof(float), (void*)(6 * sizeof(float))); // Normals
	vertexArray->assignUVAttrib(0, 3, 11 * sizeof(float), (void*)(9 * sizeof(float))); // Atlas pos
	vertexArray->translate(glm::vec3(chunk->getChunkX() * CHUNK_SIZE, chunk->getChunkY() * CHUNK_SIZE, chunk->getChunkZ() * CHUNK_SIZE));
}

long long ChunkRenderer::getColumnIndex(int chunkX, int chunkZ)
{
	// Generate Hashcode for column
	return (((long long)chunkX << (sizeof(int) * 8)) | (chunkZ & 0xffffffff));
}

ChunkRenderQueue & ChunkRenderer::getChunkRenderQueue()
{
	return chunkRenderQueue;
}

int ChunkRenderer::getDrawnedChunk()
{
	return chunkDrawed;
}

int ChunkRenderer::getChunkAmount()
{
	return chunkCount;
}
