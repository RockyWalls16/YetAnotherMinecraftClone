/*
 * WorldRenderer.h
 *
 *  Created on: 8 f�vr. 2018
 *      Author: Valentin
 */

#ifndef CLIENT_RENDER_WORLDRENDERER_H_
#define CLIENT_RENDER_WORLDRENDERER_H_

#include <client/render/ChunkRenderer.h>

enum RenderLayer;
class World;
class SkyRenderer;

class WorldRenderer
{
private:
	World* world;
	ChunkRenderer* chunkRenderer;
	SkyRenderer* skyRenderer;

	glm::vec3 sunDirection;

public:
	WorldRenderer(World* world);

	~WorldRenderer();

	void render(RenderLayer renderLayer);

	glm::vec3 getSunDirection();

	ChunkRenderer* getChunkRenderer();
	SkyRenderer* getSkyRenderer();
	World* getWorld();
};

#endif /* CLIENT_RENDER_WORLDRENDERER_H_ */