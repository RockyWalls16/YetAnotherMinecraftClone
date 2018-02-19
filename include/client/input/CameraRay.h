#ifndef CLIENT_INPUT_CAMERARAY_H_
#define CLIENT_INPUT_CAMERARAY_H_

#include <client/render/Camera.h>

class CameraRay
{
private:
	Camera* camera;

public:
	CameraRay(Camera* camera);
};

#endif