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

using namespace std;

class AirChunk;
class ChunkRenderQueue;
enum RenderLayer;

class ChunkRenderColumn
{
public:
	int chunkX;
	int chunkZ;
	std::vector<ChunkRenderIndex*> column;

	ChunkRenderColumn(int cX, int cZ) : chunkX(cX), chunkZ(cZ)
	{

	}
};

class ChunkRenderContainer
{
public:
	std::unordered_map<long long, shared_ptr<ChunkRenderColumn>> columnsMap;
};

// Contains mesh data for render
class ChunkRenderIndex
{
public:
	shared_ptr<AirChunk> chunk;
	VertexArray* chunkOpaqueMesh;
	VertexArray* chunkTransparentMesh;
	unsigned int opaqueVertexAmount;
	unsigned int transparentVertexAmount;

	ChunkRenderIndex(shared_ptr<AirChunk> chunk, VertexArray* opaqueMesh, VertexArray* transparentMesh, unsigned int opaqueVertexAmount, unsigned int transparentVertexAmount) :
		chunk(chunk),
		chunkOpaqueMesh(opaqueMesh),
		chunkTransparentMesh(transparentMesh),
		opaqueVertexAmount(opaqueVertexAmount),
		transparentVertexAmount(transparentVertexAmount)
	{};

	~ChunkRenderIndex()
	{
		delete(chunkOpaqueMesh);
		delete(chunkTransparentMesh);
	}
};

class ChunkRenderer
{
private:
	int chunkCount;
	ChunkRenderQueue* chunkRenderQueue;

	ChunkRenderContainer* opaqueContainer;
	ChunkRenderContainer* transparentContainer;

public:
	ChunkRenderer();
	~ChunkRenderer();

	void render(RenderLayer renderLayer);
	void fetchReadyChunks();
	void addChunkToRenderQueue(const shared_ptr<AirChunk>& chunk);

	VertexBuilder** prepareChunkMesh(const shared_ptr<AirChunk>& chunk);
	void applyGreedyMeshing(VertexBuilder** builders, const shared_ptr<AirChunk>& ch);
	void removeChunk(const shared_ptr<AirChunk>& chunk);
	ChunkRenderContainer* getColumnContainer(RenderLayer layer);
	shared_ptr<ChunkRenderColumn> getRenderColumn(RenderLayer layer, int x, int z);

	long long getColumnIndex(int chunkX, int chunkZ);

private:
	void configureVAO(const shared_ptr<AirChunk>& chunk, VertexBuilder* builder, VertexArray* vertexArray);
};

#endif /* CLIENT_RENDER_CHUNKRENDERER_H_ */
