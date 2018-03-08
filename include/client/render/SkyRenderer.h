#ifndef SKYRENDERER_H_
#define SKYRENDERER_H_

#include <client/textures/Texture.h>

enum RenderLayer;
class VertexArray;
class WorldRenderer;

class SkyRenderer
{
private:
	WorldRenderer* worldRenderer;
	VertexArray* skySphere;
	VertexArray* starField;
	Texture* starTexture;
	int skySphereVertexAmount;

public:
	SkyRenderer(WorldRenderer* worldRenderer);

	void render();
	void createSphere(int lats, int longs);
	void createStarField();
};

#endif