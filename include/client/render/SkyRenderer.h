#ifndef SKYRENDERER_H_
#define SKYRENDERER_H_

enum RenderLayer;
class VertexArray;
class WorldRenderer;

class SkyRenderer
{
private:
	WorldRenderer* worldRenderer;
	VertexArray* skySphere;
	int skySphereVertexAmount;

public:
	SkyRenderer(WorldRenderer* worldRenderer);

	void render();
	void createSphere(int lats, int longs);
};

#endif