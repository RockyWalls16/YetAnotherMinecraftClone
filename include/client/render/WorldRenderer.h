/*
 * WorldRenderer.h
 *
 *  Created on: 8 févr. 2018
 *      Author: Valentin
 */

#ifndef CLIENT_RENDER_WORLDRENDERER_H_
#define CLIENT_RENDER_WORLDRENDERER_H_

#include <client/render/ChunkRenderer.h>
#include <client/render/SkyRenderer.h>

enum RenderLayer;
class World;

class WorldRenderer
{
private:
	World& world;
	ChunkRenderer chunkRenderer;
	SkyRenderer skyRenderer;

public:
	WorldRenderer(World& world);

	~WorldRenderer();

	void render(RenderLayer renderLayer);

	ChunkRenderer& getChunkRenderer();
	SkyRenderer& getSkyRenderer();
	World& getWorld();
};

#endif /* CLIENT_RENDER_WORLDRENDERER_H_ */
