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

	RaycastResult(BlockAABB* blockInfo, int x, int y, int z);
	~RaycastResult();
};

class CameraRay
{
private:
	const Camera& camera;
	VertexArray* cubeHover;
	RaycastResult lookingBlock;

public:
	CameraRay(const Camera& camera);

	void tick();

	const RaycastResult& getLookingBlock();

private:
	void processLookingBlock(RaycastResult& raycastResult);

	void createCubeHoverMesh();
};

#endif