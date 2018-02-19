/*
 * ChunkRenderer.h
 *
 *  Created on: 13 sept. 2017
 *      Author: Valentin
 */

#ifndef CLIENT_RENDER_CHUNKRENDERER_H_
#define CLIENT_RENDER_CHUNKRENDERER_H_

#include <core/world/World.h>
#include <client/render/util/VertexArray.h>
#include <client/render/util/VertexBuilder.h>
#include <vector>
#include <memory>

using namespace std;

class AirChunk;
class ChunkRenderQueue;
enum RenderLayer;

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
};

class ChunkRenderer
{
private:
	ChunkRenderQueue* chunkRenderQueue;
	std::vector<ChunkRenderIndex*> opaqueChunks;
	std::vector<ChunkRenderIndex*> transparentChunks;

public:
	ChunkRenderer();
	~ChunkRenderer();

	void render(RenderLayer renderLayer);
	void fetchReadyChunks();
	void addChunkToRenderQueue(shared_ptr<AirChunk> chunk);

	VertexBuilder** prepareChunkMesh(shared_ptr<AirChunk> chunk);
	void applyGreedyMeshing(VertexBuilder** builders, shared_ptr<AirChunk> ch);
	void removeChunk(shared_ptr<AirChunk> chunk);

private:
	void configureVAO(shared_ptr<AirChunk> chunk, VertexBuilder* builder, VertexArray* vertexArray);
};

#endif /* CLIENT_RENDER_CHUNKRENDERER_H_ */
