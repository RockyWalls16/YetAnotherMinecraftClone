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

ChunkRenderer::ChunkRenderer()
{
	opaqueContainer = new ChunkRenderContainer();
	transparentContainer = new ChunkRenderContainer();

	chunkRenderQueue = new ChunkRenderQueue();
	chunkRenderQueue->start();
	chunkCount = 0;
}

ChunkRenderer::~ChunkRenderer()
{
	delete(chunkRenderQueue);
}

void ChunkRenderer::render(RenderLayer renderLayer)
{
	static int chunkDrawed = 0;
	if (renderLayer == RenderLayer::RL_OPAQUE)
	{
		chunkDrawed = 0;
	}

	ShaderCache::blockShader->use();
	TextureCache::blockTexture->bind();
	TextureCache::blockSpecularTexture->bind(1);

	fetchReadyChunks();

	World* world = Game::getInstance().getWorld();
	Camera* camera = GameRenderer::getInstance().getGameCamera();

	// Render chunks opaque then transparent
	if (renderLayer == RenderLayer::RL_OPAQUE)
	{ 
		// For each opaque column
		for (auto opaqueColumn : opaqueContainer->columnsMap)
		{
			shared_ptr<ChunkRenderColumn> column = opaqueColumn.second;
			int chunkX = column->chunkX * CHUNK_SIZE;
			int chunkZ = column->chunkZ * CHUNK_SIZE;
			
			// Check column is in frustum
			if (Frustum::columnInFrustum(chunkX, camera->getLocation().y - CHUNK_SIZE * 10, chunkZ, CHUNK_SIZE, camera->getLocation().y + CHUNK_SIZE * 10, CHUNK_SIZE))
			{
				for (ChunkRenderIndex* cri : column->column)
				{
					shared_ptr<AirChunk> chunk = cri->chunk;
					if (Frustum::boxInFrustum(chunkX, chunk->getChunkY() * CHUNK_SIZE, chunkZ, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE))
					{
						cri->chunkOpaqueMesh->drawEBO(cri->opaqueVertexAmount);
						chunkDrawed++;
					}
				}
			}
		}
	}
	else if (renderLayer == RenderLayer::RL_TRANSPARENT)
	{
		// For each opaque column
		for (auto transparentColumn : transparentContainer->columnsMap)
		{
			shared_ptr<ChunkRenderColumn> column = transparentColumn.second;
			for (ChunkRenderIndex* cri : column->column)
			{
				shared_ptr<AirChunk> chunk = cri->chunk;
				if (Frustum::boxInFrustum(chunk->getChunkX() * CHUNK_SIZE, chunk->getChunkY() * CHUNK_SIZE, chunk->getChunkZ() * CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE))
				{
					cri->chunkTransparentMesh->drawEBO(cri->transparentVertexAmount);
					chunkDrawed++;
				}
			}
		}
	}

	TextureCache::blockTexture->unbind();
}

void ChunkRenderer::fetchReadyChunks()
{
	// Check ChunkRenderQueue for ready chunk to add as VAO
	int chunkAmount = chunkRenderQueue->getOutputSize();
	for (int i = 0; i < chunkAmount; i++)
	{
		ChunkRenderOutput* cru = chunkRenderQueue->popOutputChunk();

		VertexArray* opaqueVAO = VertexArray::makeVAO();
		configureVAO(cru->chunk, cru->vertexBuilders[0], opaqueVAO);

		VertexArray* transparentVAO = VertexArray::makeVAO();
		configureVAO(cru->chunk, cru->vertexBuilders[1], transparentVAO);

		ChunkRenderIndex* cri = new ChunkRenderIndex(cru->chunk, opaqueVAO, transparentVAO, cru->vertexBuilders[0]->getIndicesWriteIndex(), cru->vertexBuilders[1]->getIndicesWriteIndex());

		// Add chunk to opaque render list
		if (cri->opaqueVertexAmount > 0)
		{
			shared_ptr<ChunkRenderColumn> opaqueColumn = getRenderColumn(RenderLayer::RL_OPAQUE, cru->chunk->getChunkX(), cru->chunk->getChunkZ());
			if (opaqueColumn == nullptr)
			{
				opaqueColumn = make_shared<ChunkRenderColumn>(cru->chunk->getChunkX(), cru->chunk->getChunkZ());
				opaqueContainer->columnsMap.insert(make_pair(ChunkColumn::getColumnIndex(cru->chunk->getChunkX(), cru->chunk->getChunkZ()), opaqueColumn));
			}
			opaqueColumn->column.push_back(cri);
			chunkCount++;
		}

		// Add chunk to transparent render list
		if (cri->transparentVertexAmount > 0)
		{
			shared_ptr<ChunkRenderColumn> transparentColumn = getRenderColumn(RenderLayer::RL_TRANSPARENT, cru->chunk->getChunkX(), cru->chunk->getChunkZ());
			if (transparentColumn == nullptr)
			{
				transparentColumn = make_shared<ChunkRenderColumn>(cru->chunk->getChunkX(), cru->chunk->getChunkZ());
				transparentContainer->columnsMap.insert(make_pair(ChunkColumn::getColumnIndex(cru->chunk->getChunkX(), cru->chunk->getChunkZ()), transparentColumn));
			}
			transparentColumn->column.push_back(cri);
			chunkCount++;
		}
		

		delete(cru);
	}
}

void ChunkRenderer::addChunkToRenderQueue(shared_ptr<AirChunk> chunk)
{
	chunkRenderQueue->pushInputChunk(chunk);
}

VertexBuilder** ChunkRenderer::prepareChunkMesh(shared_ptr<AirChunk> chunk)
{
	// Add chunks to FBO, array index [0] opaque [1] transparent
	VertexBuilder** vertexBuilders = new VertexBuilder*[2]{ new VertexBuilder(11, 12000), new VertexBuilder(11, 12000) };

	applyGreedyMeshing(vertexBuilders, chunk);

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
void ChunkRenderer::applyGreedyMeshing(VertexBuilder** builders, shared_ptr<AirChunk> ch)
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
				if (block->isVisible()) // Check block is visible
				{
					int renderLayer = block->getRenderLayer();
					for (side = 0; side < 6; side++) // For eachs side
					{
						if (!mask[x][y][z][side] && BlockRenderer::isSideVisible(ch, neighbours, block, x, y, z, (Side) side)) // Check not already meshed and side is visible
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
											if (!mask[x2][y][z2][side] && ch->getBlockAt(x2, y, z2) == block->getId() && BlockRenderer::isSideVisible(ch, neighbours, block, x2, y, z2, (Side) side))
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
											if (mask[x2][y][z2][side] || ch->getBlockAt(x2, y, z2) != block->getId() || !BlockRenderer::isSideVisible(ch, neighbours, block, x2, y, z2, (Side) side))
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

								BlockRenderer::renderFace(builders[renderLayer], block, x, y, z, x2 - x, 1, width, (Side) side);
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
											if (!mask[x][y2][z2][side] && ch->getBlockAt(x, y2, z2) == block->getId() && BlockRenderer::isSideVisible(ch, neighbours, block, x, y2, z2, (Side) side))
											{
												mask[x][y2][z2][side] = true;
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
											if (mask[x][y2][z2][side] || ch->getBlockAt(x, y2, z2) != block->getId() || !BlockRenderer::isSideVisible(ch, neighbours, block, x, y2, z2, (Side) side))
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

								BlockRenderer::renderFace(builders[renderLayer], block, x, y, z, 1, y2 - y, width, (Side)side);
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
											if (!mask[x2][y2][z][side] && ch->getBlockAt(x2, y2, z) == block->getId() && BlockRenderer::isSideVisible(ch, neighbours, block, x2, y2, z, (Side) side))
											{
												mask[x2][y2][z][side] = true;
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
											if (mask[x2][y2][z][side] || ch->getBlockAt(x2, y2, z) != block->getId() || !BlockRenderer::isSideVisible(ch, neighbours, block, x2, y2, z, (Side) side))
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

								BlockRenderer::renderFace(builders[renderLayer], block, x, y, z, x2 - x, height, 1, (Side)side);
							}
						}
					}
				}
			}
		}
	}
}

void ChunkRenderer::removeChunk(shared_ptr<AirChunk> chunk)
{
	// Remove opaque chunks render 
	shared_ptr<ChunkRenderColumn> opaqueColumn = getRenderColumn(RenderLayer::RL_OPAQUE, chunk->getChunkX(), chunk->getChunkZ());
	if (opaqueColumn != nullptr)
	{
		// Iterate over column
		vector<ChunkRenderIndex*>::iterator iter;
		for (iter = opaqueColumn->column.begin(); iter != opaqueColumn->column.end();)
		{
			if ((*iter)->chunk == chunk)
			{
				ChunkRenderIndex* cri = *iter;

				// If chunks also contains transparent vertices delete it
				if (cri->transparentVertexAmount > 0)
				{
					shared_ptr<ChunkRenderColumn> transparentColumn = getRenderColumn(RenderLayer::RL_TRANSPARENT, chunk->getChunkX(), chunk->getChunkZ());
					transparentColumn->column.erase(remove(transparentColumn->column.begin(), transparentColumn->column.end(), cri), transparentColumn->column.end());

					// Free transparent column memory
					if (transparentColumn->column.empty())
					{
						transparentContainer->columnsMap.erase(ChunkColumn::getColumnIndex(chunk->getChunkX(), chunk->getChunkZ()));
						chunkCount--;
					}
				}

				delete(*iter);
				iter = opaqueColumn->column.erase(iter);
				chunkCount--;

				// Free opaque column memory
				if (opaqueColumn->column.empty())
				{
					opaqueContainer->columnsMap.erase(ChunkColumn::getColumnIndex(chunk->getChunkX(), chunk->getChunkZ()));
				}

				return;
			}
			else
			{
				iter++;
			}
		}
	}

	

	// If chunk is composed ONLY of transparent blocks
	shared_ptr<ChunkRenderColumn> transparentColumn = getRenderColumn(RenderLayer::RL_TRANSPARENT, chunk->getChunkX(), chunk->getChunkZ());
	if (transparentColumn != nullptr)
	{
		// Iterate over column
		vector<ChunkRenderIndex*>::iterator iter;
		for (iter = transparentColumn->column.begin(); iter != transparentColumn->column.end();)
		{
			if ((*iter)->chunk == chunk)
			{
				ChunkRenderIndex* cri = *iter;

				delete(*iter);
				iter = transparentColumn->column.erase(iter);
				chunkCount--;

				// Free transparent column memory
				if (transparentColumn->column.empty())
				{
					transparentContainer->columnsMap.erase(ChunkColumn::getColumnIndex(chunk->getChunkX(), chunk->getChunkZ()));
				}

				return;
			}
			else
			{
				iter++;
			}
		}
	}
}

ChunkRenderContainer * ChunkRenderer::getColumnContainer(RenderLayer layer)
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
	ChunkRenderContainer* column = getColumnContainer(layer);
	auto it = column->columnsMap.find(ChunkColumn::getColumnIndex(x, z));
	return it != column->columnsMap.end() ? it->second : nullptr;
}

void ChunkRenderer::configureVAO(shared_ptr<AirChunk> chunk, VertexBuilder * builder, VertexArray * vertexArray)
{
	vertexArray->addVBO(builder->getVertexBuffer(), builder->getVBOSize(), GL_STATIC_DRAW);
	vertexArray->enableEBO(builder->getIndicesBuffer(), builder->getEOBSize(), GL_STATIC_DRAW);
	vertexArray->assignPositionAttrib(0, 0, 11 * sizeof(float)); // Position
	vertexArray->assignRGBAttrib(0, 1, 11 * sizeof(float), (void*)(3 * sizeof(float))); // Colors
	vertexArray->assignPositionAttrib(0, 2, 11 * sizeof(float), (void*)(6 * sizeof(float))); // Normals
	vertexArray->assignUVAttrib(0, 3, 11 * sizeof(float), (void*)(9 * sizeof(float))); // Atlas pos
	vertexArray->translate(glm::vec3(chunk->getChunkX() * CHUNK_SIZE, chunk->getChunkY() * CHUNK_SIZE, chunk->getChunkZ() * CHUNK_SIZE));
}
