#include <client/input/CameraRay.h>
#include <client/shaders/ShaderCache.h>
#include <client/render/Camera.h>
#include <Game.h>
#include <math/AABB.h>
#include <util/Logger.h>
#include <glm/gtc/constants.hpp>

CameraRay::CameraRay(const Camera& camera) : camera(camera), lookingBlock(RaycastResult(nullptr, 0, 0, 0))
{}

void CameraRay::initCameraRay()
{
	createCubeHoverMesh();
}

void CameraRay::tick()
{
	processLookingBlock(lookingBlock);
	
	if (lookingBlock.blockInfo)
	{
		// Draw cursor
		ShaderCache::unlitShader->use();

		cubeHover->setIdentity();
		cubeHover->translate(glm::vec3(lookingBlock.blockInfo->x + 0.5F, lookingBlock.blockInfo->y + 0.5F, lookingBlock.blockInfo->z + 0.5F));

		glLineWidth(1.5F);
		cubeHover->drawVAO(48, 0, GL_LINES);
	}
}

const RaycastResult& CameraRay::getLookingBlock() const
{
	return lookingBlock;
}

void CameraRay::processLookingBlock(RaycastResult& raycastResult)
{
	// Get currently looking block
	float minDistance = PLACE_DISTANCE + 1;
	glm::vec3 cameraPos = camera.getLocation();
	glm::vec3 forward = camera.getForwardVec();
	glm::vec3 forwardExpand = glm::vec3(forward.x * PLACE_DISTANCE, forward.y * PLACE_DISTANCE, forward.z * PLACE_DISTANCE);
	AABB lookBox = AABB(glm::vec3(cameraPos.x, cameraPos.y, cameraPos.z), glm::vec3(cameraPos.x, cameraPos.y, cameraPos.z)).expandBox(glm::vec3(forwardExpand.x, forwardExpand.y, forwardExpand.z));
	
	std::vector<BlockAABB> tiles;
	lookBox.blockInfoInBox(*Game::getInstance().getWorld(), &tiles);
	BlockAABB* nearestBlock = nullptr;
	glm::vec3 nearestIntersect;
	glm::vec3 nearestNormal;
	glm::vec3 outIntersect;
	glm::vec3 outNormal;

	// Check for each tile in camera sight line
	for(BlockAABB& tile : tiles)
	{
		bool hit = tile.aabb.intersectLine(cameraPos, cameraPos + forwardExpand, &outIntersect, &outNormal);

		// Block hit
		if (hit)
		{
			float distance = glm::distance(cameraPos, outIntersect);
			if (distance < minDistance)
			{
				minDistance = distance;
				nearestIntersect = outIntersect;
				nearestNormal = outNormal;
				nearestBlock = new BlockAABB(tile);
			}
		}
	}

	if (raycastResult.blockInfo)
	{
		delete(raycastResult.blockInfo);
	}

	raycastResult.blockInfo = nearestBlock;
	raycastResult.nX = (int) nearestNormal.x;
	raycastResult.nY = (int) nearestNormal.y;
	raycastResult.nZ = (int) nearestNormal.z;
}

void CameraRay::createCubeHoverMesh()
{
	float vertices[] =
	{
		// Top face
		-0.5F, 0.5F, -0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C
		0.5F, 0.5F, -0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C

		0.5F, 0.5F, -0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C
		0.5F, 0.5F, 0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C

		0.5F, 0.5F, 0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C
		-0.5F, 0.5F, 0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C

		-0.5F, 0.5F, 0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C
		-0.5F, 0.5F, -0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C

		// Bottom face
		-0.5F, -0.5F, -0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C
		0.5F, -0.5F, -0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C

		0.5F, -0.5F, -0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C
		0.5F, -0.5F, 0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C

		0.5F, -0.5F, 0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C
		-0.5F, -0.5F, 0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C

		-0.5F, -0.5F, 0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C
		-0.5F, -0.5F, -0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C

		// X- face
		-0.5F, -0.5F, -0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C
		-0.5F, 0.5F, -0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C

		-0.5F, 0.5F, -0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C
		-0.5F, 0.5F, 0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C

		-0.5F, 0.5F, 0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C
		-0.5F, -0.5F, 0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C

		-0.5F, -0.5F, 0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C
		-0.5F, -0.5F, -0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C

		// X+ face
		0.5F, -0.5F, -0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C
		0.5F, 0.5F, -0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C

		0.5F, 0.5F, -0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C
		0.5F, 0.5F, 0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C

		0.5F, 0.5F, 0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C
		0.5F, -0.5F, 0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C

		0.5F, -0.5F, 0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C
		0.5F, -0.5F, -0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C

		// Z- face
		-0.5F, -0.5F, -0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C
		-0.5F, 0.5F, -0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C

		-0.5F, 0.5F, -0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C
		0.5F, 0.5F, -0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C

		0.5F, 0.5F, -0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C
		0.5F, -0.5F, -0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C

		0.5F, -0.5F, -0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C
		-0.5F, -0.5F, -0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C

		// Z+ face
		-0.5F, -0.5F, 0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C
		-0.5F, 0.5F, 0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C

		-0.5F, 0.5F, 0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C
		0.5F, 0.5F, 0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C

		0.5F, 0.5F, 0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C
		0.5F, -0.5F, 0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C

		0.5F, -0.5F, 0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C
		-0.5F, -0.5F, 0.5F, //P
		0.0F, 0.0F, 0.0F, 0.5F, // C
	};

	cubeHover = new VertexArray();
	cubeHover->disableNormals();
	cubeHover->addVBO(vertices, 336 * sizeof(float), GL_STATIC_DRAW);
	cubeHover->assignPositionAttrib(0, 0, sizeof(float) * 7);
	cubeHover->assignRGBAAttrib(0, 1, sizeof(float) * 7, (void*) (sizeof(float) * 3));
}

RaycastResult::RaycastResult(BlockAABB * blockInfo, int x, int y, int z) : blockInfo(blockInfo), nX(x), nY(y), nZ(z) {}

RaycastResult::~RaycastResult()
{
	if (blockInfo != nullptr)
	{
		delete(blockInfo);
	}
}
