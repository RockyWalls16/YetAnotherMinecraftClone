#ifndef CLIENT_INPUT_CAMERARAY_H_
#define CLIENT_INPUT_CAMERARAY_H_

#include <client/render/util/VertexArray.h>

#define PLACE_DISTANCE 5

class BlockAABB;
class Camera;

class CameraRay
{
private:
	Camera* camera;
	VertexArray* cubeHover;

public:
	CameraRay(Camera* camera);

	void tick();

private:
	BlockAABB* getLookingBlock();

	void createCubeHoverMesh();
};

#endif