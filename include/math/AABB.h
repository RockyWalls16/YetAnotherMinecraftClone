#ifndef MATH_AABB_H_
#define MATH_AABB_H_

#include <glm/vec3.hpp>
#include <vector>

class Block;
class World;

class AABB
{
public:
	glm::vec3 startPos;
	glm::vec3 endPos;

public:
	AABB(glm::vec3 startPos, glm::vec3 endPos);

	void updatePos(glm::vec3 newPos);

	void updatePosCenter(glm::vec3 newPos);

	void move(glm::vec3 amount);

	std::vector<AABB*>* tilesInBox(World* world);

	AABB* expandBox(glm::vec3 expandAABB);

	void clipX(AABB* hitbox, float* velocityX);

	void clipY(AABB* hitbox, float* velocityY);

	void clipZ(AABB* hitbox, float* velocityZ);
};

#endif