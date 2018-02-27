#ifndef MATH_FRUSTUM_H_
#define MATH_FRUSTUM_H_

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

class Camera;

enum PlaneOrientation
{
	TOPP,
	BOTTOMP,
	LEFTP,
	RIGHTP,
	NEARP,
	FARP
};

class Plane
{
public:
	glm::vec3 point;
	glm::vec3 normal;
	float d;

	void setPN(glm::vec3& point, glm::vec3& normal)
	{
		this->point = point;
		this->normal = normal;
		d = -glm::dot(normal, point);
	}

	float Plane::distance(glm::vec3 &p)
	{
		return (d + glm::dot(normal, p));
	}
};

class Frustum
{
private:

	static Plane planes[6];
	static float nearH;
	static float nearW;
	static float farH;
	static float farW;

public:
	static void updateFrustumSize(Camera& camera);

	static void computePlanes(Camera& camera);
	static bool pointInFrustum(glm::vec3 & p);
	static bool boxInFrustum(int x, int y, int z, int sX, int sY, int sZ);
	static bool columnInFrustum(int x, int y, int z, int sX, int sY, int sZ);
};

#endif