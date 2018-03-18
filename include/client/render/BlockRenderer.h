/*
 * BlockRenderer.h
 *
 *  Created on: 8 févr. 2018
 *      Author: Valentin
 */

#ifndef CLIENT_RENDER_BLOCKRENDERER_H_
#define CLIENT_RENDER_BLOCKRENDERER_H_

#define ATLAS_COLUMNS 16
#define ATLAS_ROWS 16

#include <core/world/AirChunk.h>
#include <client/render/util/VertexBuilder.h>
#include <memory>

using namespace std;

enum RenderType
{
	BLOCK,
	CROSS
};

class Block;

class BlockRenderer
{
private:
	static unsigned int indices[];
	static float blockVertices[];
	static float crossVertices[];
	static float atlasCellW;
	static float atlasCellH;

public:
	static void initBlockRenderer();

	static bool isSideVisible(const shared_ptr<AirChunk>& chunk, shared_ptr<AirChunk> neighbours[6], Block* blockId, int x, int y, int z, Side faceSide);

	static bool shallRenderFace(const shared_ptr<AirChunk>& chunk, shared_ptr<AirChunk> neighbours[6], Block* blockId, int x, int y, int z, Side faceSide);

	static short getBlockAtWithNeighbours(const shared_ptr<AirChunk>& chunk, shared_ptr<AirChunk> neighbours[6], Side side, int x, int y, int z);

	static void renderFace(VertexBuilder* vertexBuilder, Block* blockId, AABB& renderBox, int x, int y, int z, int sX, int sY, int sZ, Side faceSide);

	static void renderSimpleFace(VertexBuilder* vertexBuilder, Block* blockId, AABB& renderBox, int x, int y, int z, Side faceSide);

	static void renderBlock(VertexBuilder* vertexBuilder, Block* blockId, RenderType renderType, int x, int y, int z);
};

#endif /* CLIENT_RENDER_BLOCKRENDERER_H_ */
