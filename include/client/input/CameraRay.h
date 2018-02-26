#ifndef CLIENT_INPUT_CAMERARAY_H_
#define CLIENT_INPUT_CAMERARAY_H_

#include <client/render/util/VertexArray.h>

#define PLACE_DISTANCE 5

class BlockAABB;
class Camera;

class RaycastResult
{
public:
	BlockAABB * blockInfo;
	int nX;
	int nY;
	int nZ;

	RaycastResult(BlockAABB* blockInfo, int x, int y, int z) : blockInfo(blockInfo), nX(x), nY(y), nZ(z) {}
	~RaycastResult()
	{
		if (blockInfo != nullptr)
		{
			delete(blockInfo);
		}
	};
};

class CameraRay
{
private:
	Camera* camera;
	VertexArray* cubeHover;
	RaycastResult* lookingBlock;

public:
	CameraRay(Camera* camera);

	void tick();

	RaycastResult* getLookingBlock();

private:
	RaycastResult* processLookingBlock();

	void createCubeHoverMesh();
};

#endif