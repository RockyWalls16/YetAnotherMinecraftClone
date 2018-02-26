#ifndef MATH_AABB_H_
#define MATH_AABB_H_

#include <glm/vec3.hpp>
#include <vector>

class Block;
class World;
class BlockAABB;


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

	void blockInBox(World* world, std::vector<AABB>* aabbVector);

	void blockInfoInBox(World* world, std::vector<BlockAABB*>* aabbVector);

	AABB expandBox(glm::vec3 expandAABB);

	void clipX(AABB* hitbox, float* velocityX);

	void clipY(AABB* hitbox, float* velocityY);

	void clipZ(AABB* hitbox, float* velocityZ);

	bool intersectLine(glm::vec3 startVec, glm::vec3 endVector, glm::vec3* outIntersect, glm::vec3* outNormal);

	bool clipLine(float x1, float x2, float startVecX, float endVectorX, float* fLow, float* fHigh);
};

class BlockAABB
{
public:
	Block * block;
	AABB aabb;
	int x;
	int y;
	int z;

	BlockAABB(Block* block, AABB aabb, int x, int y, int z) : block(block), aabb(aabb), x(x), y(y), z(z) {}
};

#endif