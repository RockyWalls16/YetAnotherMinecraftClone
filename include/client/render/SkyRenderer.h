#ifndef SKYRENDERER_H_
#define SKYRENDERER_H_

#include <client/textures/Texture.h>
#include <client/render/light/DirectionalLight.h>
#include <client/render/util/VertexArray.h>

enum RenderLayer;
class WorldRenderer;

class SkyRenderer
{
private:
	WorldRenderer& worldRenderer;
	VertexArray skySphere;
	VertexArray starField;
	Texture* starTexture;
	int skySphereVertexAmount;

	DirectionalLight sunLight;
	DirectionalLight moonLight;

public:
	SkyRenderer(WorldRenderer& worldRenderer);

	void render();
	void createSphere(int lats, int longs);
	void createStarField();
};

#endif