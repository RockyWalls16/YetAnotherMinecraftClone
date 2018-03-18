/*
 * ChunkRenderer.h
 *
 *  Created on: 13 sept. 2017
 *      Author: Valentin
 */

#ifndef CLIENT_RENDER_CHUNKRENDERER_H_
#define CLIENT_RENDER_CHUNKRENDERER_H_

#include <core/world/World.h>
#include <util/Logger.h>
#include <client/render/util/VertexArray.h>
#include <client/render/util/VertexBuilder.h>
#include <vector>
#include <unordered_map>
#include <memory>
#include <client/render/ChunkRenderQueue.h>
#include <sparsepp/spp.h>

using namespace std;

class AirChunk;
class WorldRenderer;
enum RenderLayer;

class ChunkRenderColumn
{
public:
	int chunkX;
	int chunkZ;
	std::vector<ChunkRenderIndex*> column;

	ChunkRenderColumn(int cX, int cZ) : chunkX(cX), chunkZ(cZ)
	{}
};

class ChunkRenderContainer
{
public:
	spp::sparse_hash_map<long long, shared_ptr<ChunkRenderColumn>> columnsMap;
};

// Contains mesh data for render
class ChunkRenderIndex
{
public:
	shared_ptr<AirChunk> chunk;
	VertexArray* mesh;
	unsigned int vertexAmount;

	ChunkRenderIndex(shared_ptr<AirChunk> chunk, VertexArray* mesh, unsigned int vertexAmount) :
		chunk(chunk),
		mesh(mesh),
		vertexAmount(vertexAmount)
	{};

	~ChunkRenderIndex()
	{
		delete(mesh);
	}
};

class ChunkRenderer
{
private:
	int chunkCount;
	int chunkDrawed;
	ChunkRenderQueue chunkRenderQueue;
	ChunkRenderContainer opaqueContainer;
	ChunkRenderContainer transparentContainer;

public:
	ChunkRenderer();
	~ChunkRenderer();

	void render(RenderLayer renderLayer);

	void renderChunkLayer(RenderLayer renderLayer);

	void fetchReadyChunks();
	void addChunkToContainer(ChunkRenderContainer& columnContainer, shared_ptr<ChunkRenderColumn> layerColumn, shared_ptr<AirChunk>& chunk, VertexBuilder* builders, int vertexAmount);
	void addChunkToRenderQueue(const shared_ptr<AirChunk>& chunk);

	VertexBuilder** prepareChunkMesh(const shared_ptr<AirChunk>& chunk);
	void applyGreedyMeshing(VertexBuilder** builders, const shared_ptr<AirChunk>& ch);
	void applyStandardMesh(VertexBuilder** builders, const shared_ptr<AirChunk>& ch);
	void removeChunk(const shared_ptr<AirChunk>& chunk);
	void removeChunkAtLayer(const shared_ptr<AirChunk>& chunk, RenderLayer renderLayer);
	ChunkRenderContainer& getColumnContainer(RenderLayer layer);
	shared_ptr<ChunkRenderColumn> getRenderColumn(RenderLayer layer, int x, int z);

	long long getColumnIndex(int chunkX, int chunkZ);

	ChunkRenderQueue& getChunkRenderQueue();

private:
	void configureVAO(const shared_ptr<AirChunk>& chunk, VertexBuilder* builder, VertexArray* vertexArray);
};

#endif /* CLIENT_RENDER_CHUNKRENDERER_H_ */
