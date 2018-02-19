/*
 * WorldRenderer.cpp
 *
 *  Created on: 8 f�vr. 2018
 *      Author: Valentin
 */

#include <core/block/Block.h>
#include <client/render/WorldRenderer.h>
#include <client/render/BlockRenderer.h>
#include <client/textures/TextureCache.h>
#include <client/textures/TextureLoader.h>
#include <client/render/GameRenderer.h>
#include <client/render/SkyRenderer.h>
#include <core/world/World.h>

WorldRenderer::WorldRenderer(World* world) : world(world)
{
	BlockRenderer::initBlockRenderer();

	chunkRenderer = new ChunkRenderer();
	sunDirection = glm::vec3(-50.0F, 300, -100.0F);

	skyRenderer = new SkyRenderer(this);
}

WorldRenderer::~WorldRenderer()
{
	delete(chunkRenderer);
}

void WorldRenderer::render(RenderLayer renderLayer)
{
	skyRenderer->render();
	chunkRenderer->render(renderLayer);
}

glm::vec3 WorldRenderer::getSunDirection()
{
	return sunDirection;
}

ChunkRenderer * WorldRenderer::getChunkRenderer()
{
	return chunkRenderer;
}

SkyRenderer * WorldRenderer::getSkyRenderer()
{
	return skyRenderer;
}

World * WorldRenderer::getWorld()
{
	return world;
}