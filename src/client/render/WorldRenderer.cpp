/*
 * WorldRenderer.cpp
 *
 *  Created on: 8 févr. 2018
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

WorldRenderer::WorldRenderer(World& world) : world(world), skyRenderer(SkyRenderer(*this))
{
	BlockRenderer::initBlockRenderer();
}

WorldRenderer::~WorldRenderer()
{
	
}

void WorldRenderer::render(RenderLayer renderLayer)
{
	// Render sky
	if (renderLayer == RenderLayer::RL_PRE_PP)
	{
		skyRenderer.render();
	}

	// Render chunks
	if (renderLayer <= RenderLayer::RL_TRANSPARENT)
	{
		chunkRenderer.render(renderLayer);
	}
}

ChunkRenderer& WorldRenderer::getChunkRenderer()
{
	return chunkRenderer;
}

SkyRenderer& WorldRenderer::getSkyRenderer()
{
	return skyRenderer;
}

World& WorldRenderer::getWorld()
{
	return world;
}
