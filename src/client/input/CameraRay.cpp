#include <client/input/CameraRay.h>
#include <client/shaders/ShaderCache.h>
#include <client/render/Camera.h>
#include <Game.h>
#include <math/AABB.h>
#include <util/Logger.h>
#include <glm/gtc/constants.hpp>


int skySphereVertexAmount = 0;

CameraRay::CameraRay(Camera * camera) : camera(camera)
{
	createCubeHoverMesh();
}

void CameraRay::tick()
{
	BlockAABB* lookingBlock = getLookingBlock();
	
	// Free memory
	if (lookingBlock)
	{
		ShaderCache::skyShader->use();

		cubeHover->setIdentity();
		cubeHover->translate(glm::vec3(lookingBlock->x + 0.5F, lookingBlock->y + 0.5F, lookingBlock->z + 0.5F));
		cubeHover->scale(glm::vec3(1.1F, 1.1F, 1.1F));

		glDisable(GL_CULL_FACE);
		cubeHover->drawVAO(skySphereVertexAmount, 0, GL_TRIANGLE_FAN);
		glEnable(GL_CULL_FACE);

		delete(lookingBlock);
	}
}

BlockAABB* CameraRay::getLookingBlock()
{
	// Get currently looking block
	float minDistance = PLACE_DISTANCE + 1;
	glm::vec3 cameraPos = camera->getLocation();
	glm::vec3 forward = camera->getForwardVec();
	glm::vec3 forwardExpand = glm::vec3(forward.x * PLACE_DISTANCE, forward.y * PLACE_DISTANCE, forward.z * PLACE_DISTANCE);
	AABB lookBox = AABB(glm::vec3(cameraPos.x, cameraPos.y, cameraPos.z), glm::vec3(cameraPos.x, cameraPos.y, cameraPos.z)).expandBox(glm::vec3(forwardExpand.x, forwardExpand.y, forwardExpand.z));
	
	std::vector<BlockAABB*> tiles;
	lookBox.blockInfoInBox(Game::getInstance().getWorld(), &tiles);
	BlockAABB* nearestBlock = nullptr;
	glm::vec3 nearestIntersect;
	glm::vec3 nearestNormal;
	glm::vec3 outIntersect;
	glm::vec3 outNormal;

	// Check for each tile in camera sight line
	for(BlockAABB* tile : tiles)
	{
		bool hit = tile->aabb.intersectLine(cameraPos, cameraPos + forwardExpand, &outIntersect, &outNormal);

		// Block hit
		if (hit)
		{
			float distance = (cameraPos - outIntersect).length();
			if (distance < minDistance)
			{
				minDistance = distance;
				nearestIntersect = outIntersect;
				nearestNormal = outNormal;
				nearestBlock = tile;
			}
		}
	}

	// Free memory
	for (int i = 0, length = tiles.size(); i < length; i++)
	{
		if (nearestBlock != tiles[i])
		{
			delete(tiles[i]);
		}
	}

	return nearestBlock;
}

VertexArray* createSphere(int lats, int longs);

void CameraRay::createCubeHoverMesh()
{
	cubeHover = createSphere(15, 15);
}

VertexArray* createSphere(int lats, int longs)
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

	VertexArray* skySphere = VertexArray::makeVAO();
	skySphere->disableNormals();
	skySphere->addVBO(vertices, vertexAmount * sizeof(float), GL_STATIC_DRAW);
	skySphere->assignPositionAttrib(0, 0, sizeof(float) * 3);

	skySphereVertexAmount = vertexAmount;

	return skySphere;
}
