#include <math/Frustum.h>
#include <client/render/Camera.h>
#include <glm/trigonometric.hpp>
#include <util/Logger.h>
#include <string>

float Frustum::nearH;
float Frustum::nearW;
float Frustum::farH;
float Frustum::farW;
Plane Frustum::planes[6];

void Frustum::updateFrustumSize(Camera * camera)
{
	// Calculate frustum plane Width & Height
	float tanV = tan(glm::radians(camera->getFov() * 0.5F));
	nearH = 2.0F * NEAR_PLANE * tanV;
	farH = 2.0F * FAR_PLANE * tanV;
	nearW = nearH * camera->getAspect();
	farW = farH * camera->getAspect();
}

void Frustum::computePlanes(Camera * camera)
{
	glm::vec3 pos = camera->getLocation();
	glm::vec3 forward = camera->getForwardVec();
	glm::vec3 up = camera->getUpVec();
	glm::vec3 right = camera->getStrafeVec();

	// Looking direction
	glm::vec3 z = -forward;

	// X axis of camera with given "up" vector and Z axis
	glm::vec3 x = right;

	// the real "up" vector is the cross product of Z and X
	glm::vec3 y = up;

	glm::vec3 nearPlane = pos + forward * NEAR_PLANE;
	glm::vec3 farPlane = pos + forward * FAR_PLANE;

	planes[NEARP].setPN(nearPlane, -z);
	planes[FARP].setPN(farPlane, z);

	// Calculate planes
	glm::vec3 aux, normal;
	aux = glm::normalize((nearPlane + y * nearH) - pos);
	normal = glm::cross(aux, x);
	planes[TOPP].setPN(nearPlane + y * nearH, normal);

	aux = glm::normalize((nearPlane - y * nearH) - pos);
	normal = glm::cross(x, aux);
	planes[BOTTOMP].setPN(nearPlane - y * nearH, normal);

	aux = glm::normalize((nearPlane - x * nearW) - pos);
	normal = glm::cross(aux, y);
	planes[LEFTP].setPN(nearPlane - x * nearW, normal);

	aux = glm::normalize((nearPlane + x * nearW) - pos);
	normal = glm::cross(y, aux);
	planes[RIGHTP].setPN(nearPlane + x * nearW, normal);
}

bool Frustum::pointInFrustum(glm::vec3 &p)
{
	// For each frustum plane
	for (int i = 0; i < 6; i++)
	{
		if (planes[i].distance(p) < 0)
		{
			return false;
		}
	}
	return true;
}

bool Frustum::boxInFrustum(int x, int y, int z, int sX, int sY, int sZ)
{
	// For each frustum plane
	for (int i = 0; i < 6; i++)
	{
		glm::vec3 planeNormal = planes[i].normal;
		glm::vec3 origin = glm::vec3(x, y, z);

		// Move origin
		if (planeNormal.x > 0)
		{
			origin.x += sX;
		}
		
		if (planeNormal.y > 0)
		{
			origin.y += sY;
		}

		if (planeNormal.z > 0)
		{
			origin.z += sZ;
		}

		// Check point is outside frustum
		if (planes[i].distance(origin) < 0)
		{
			return false;
		}
	}
	return true;
}