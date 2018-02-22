#include <client/render/SkyRenderer.h>
#include <client/shaders/ShaderCache.h>
#include <client/render/WorldRenderer.h>
#include <client/render/util/VertexBuilder.h>
#include <client/render/util/VertexArray.h>
#include <glm/gtc/constants.hpp>
#include <util/Logger.h>
#include <client/render/Camera.h>
#include <client/render/GameRenderer.h>

SkyRenderer::SkyRenderer(WorldRenderer* worldRenderer) : worldRenderer(worldRenderer)
{
	createSphere(6, 6);
}

void SkyRenderer::render()
{
	ShaderCache::skyShader->use();
	skySphere->setIdentity();
	skySphere->translate(glm::vec3(GameRenderer::getInstance().getGameCamera()->getLocation()));
	skySphere->scale(glm::vec3(FAR_PLANE - 1, FAR_PLANE - 1, FAR_PLANE - 1));
	skySphere->drawVAO(skySphereVertexAmount, 0, GL_TRIANGLE_STRIP);
}

void SkyRenderer::createSphere(int lats, int longs)
{
	int vertexAmount = (lats + 1) * (longs + 1) * 6;
	float* vertices = new float[vertexAmount];

	int currentVertex = 0;
	for (int i = 0; i <= lats; i++)
	{
		float lat0 = glm::pi<float>() * (-0.5F + (float)(i - 1) / lats);
		float z0 = sin(lat0);
		float zr0 = cos(lat0);

		float lat1 = glm::pi<float>() * (-0.5F + (float)i / lats);
		float z1 = sin(lat1);
		float zr1 = cos(lat1);

		for (int j = 0; j <= longs; j++)
		{
			float lng = 2 * glm::pi<float>() * (float)(j - 1) / longs;
			float x = cos(lng);
			float y = sin(lng);

			vertices[currentVertex] = x * zr0;
			vertices[currentVertex + 1] = y * zr0;
			vertices[currentVertex + 2] = z0;

			vertices[currentVertex + 3] = x * zr1;
			vertices[currentVertex + 4] = y * zr1;
			vertices[currentVertex + 5] = z1;

			currentVertex += 6;
		}
	}

	skySphere = VertexArray::makeVAO();
	skySphere->disableNormals();
	skySphere->addVBO(vertices, vertexAmount * sizeof(float), GL_STATIC_DRAW);
	skySphere->assignPositionAttrib(0, 0, sizeof(float) * 3);
	skySphere->scale(glm::vec3(FAR_PLANE - 1, FAR_PLANE - 1, FAR_PLANE - 1));
	
	skySphereVertexAmount = vertexAmount;
}
