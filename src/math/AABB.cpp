#include <math/AABB.h>
#include <math/MathUtil.h>
#include <core/block/Block.h>
#include <core/world/World.h>
#include <cmath>
#include <algorithm>
#include <util/Side.h>

AABB::AABB(glm::vec3 startPos, glm::vec3 endPos) : startPos(startPos), endPos(endPos)
{
}

void AABB::updatePos(glm::vec3 newPos)
{
	// Move AABB to new pos from Corner
	glm::vec3 size = endPos - startPos;

	startPos = newPos;
	endPos = newPos + size;
}

void AABB::updatePosCenter(glm::vec3 newPos)
{
	// Move AABB to new pos from Bottom Center
	glm::vec3 size = endPos - startPos;
	glm::vec3 halfSize = size;
	halfSize.x /= 2.0F;
	halfSize.z /= 2.0F;

	startPos = newPos;
	startPos.x -= halfSize.x;
	startPos.z -= halfSize.z;

	endPos = newPos + halfSize;
}

void AABB::move(glm::vec3 amount)
{
	startPos += amount;
	endPos += amount;
}

void AABB::blockInBox(World* world, std::vector<AABB>* aabbVector)
{
	int x, y, z;
	int bX = (int) floor(startPos.x);
	int bY = (int) floor(startPos.y);
	int bZ = (int) floor(startPos.z);
	int bX2 = (int) ceil(endPos.x);
	int bY2 = (int) ceil(endPos.y);
	int bZ2 = (int) ceil(endPos.z);
	Block* block;

	for (y = bY; y < bY2; y++)
	{
		for (x = bX; x < bX2; x++)
		{
			for (z = bZ; z < bZ2; z++)
			{
				block = world->getBlockAt(x, y, z);

				if (block->canCollide())
				{
					aabbVector->push_back(block->getHitbox(x, y, z));
				}
			}
		}
	}
}

void AABB::blockInfoInBox(World* world, std::vector<BlockAABB*>* aabbVector)
{
	int x, y, z;
	int bX = (int)floor(startPos.x);
	int bY = (int)floor(startPos.y);
	int bZ = (int)floor(startPos.z);
	int bX2 = (int)ceil(endPos.x);
	int bY2 = (int)ceil(endPos.y);
	int bZ2 = (int)ceil(endPos.z);
	Block* block;

	for (y = bY; y < bY2; y++)
	{
		for (x = bX; x < bX2; x++)
		{
			for (z = bZ; z < bZ2; z++)
			{
				block = world->getBlockAt(x, y, z);

				if (block->canCollide())
				{
					aabbVector->push_back(new BlockAABB(block, block->getHitbox(x, y, z), x, y, z));
				}
			}
		}
	}
}

AABB AABB::expandBox(glm::vec3 expandAABB)
{
	AABB newBox = AABB(startPos, endPos);

	// Expand X
	if (expandAABB.x < 0)
	{
		newBox.startPos.x += expandAABB.x;
	}
	else
	{
		newBox.endPos.x += expandAABB.x;
	}

	// Expand Y
	if (expandAABB.y < 0)
	{
		newBox.startPos.y += expandAABB.y;
	}
	else
	{
		newBox.endPos.y += expandAABB.y;
	}

	// Expand Z
	if (expandAABB.z < 0)
	{
		newBox.startPos.z += expandAABB.z;
	}
	else
	{
		newBox.endPos.z += expandAABB.z;
	}

	return newBox;
}

void AABB::clipX(AABB* hitbox, float* velocityX)
{
	//Check can collide in YZ axis
	if (hitbox->startPos.y >= endPos.y || hitbox->endPos.y <= startPos.y || hitbox->startPos.z >= endPos.z || hitbox->endPos.z <= startPos.z)
	{
		return;
	}
	
	// Check X- face collision
	if (*velocityX > 0 && hitbox->endPos.x <= startPos.x)
	{
		float distance = startPos.x - hitbox->endPos.x;
		*velocityX = distance < *velocityX ? distance : *velocityX;
	}
	// Check X+ face collision
	else if (*velocityX < 0 && hitbox->startPos.x >= endPos.x)
	{
		float distance = endPos.x - hitbox->startPos.x;
		*velocityX = distance > *velocityX ? distance : *velocityX;
	}
}

void AABB::clipY(AABB* hitbox, float* velocityY)
{
	//Check can collide in XZ axis
	if (hitbox->startPos.x >= endPos.x || hitbox->endPos.x <= startPos.x || hitbox->startPos.z >= endPos.z || hitbox->endPos.z <= startPos.z)
	{
		return;
	}

	// Check bottom face collision
	if (*velocityY > 0 && hitbox->endPos.y <= startPos.y)
	{
		float distance = startPos.y - hitbox->endPos.y;
		*velocityY = distance < *velocityY ? distance : *velocityY;
	}
	// Check top face collision
	else if (*velocityY < 0 && hitbox->startPos.y >= endPos.y)
	{
		float distance = endPos.y - hitbox->startPos.y;
		*velocityY = distance > *velocityY ? distance : *velocityY;
	}
}

void AABB::clipZ(AABB* hitbox, float* velocityZ)
{
	//Check can collide in YX axis
	if (hitbox->startPos.y >= endPos.y || hitbox->endPos.y <= startPos.y || hitbox->startPos.x >= endPos.x || hitbox->endPos.x <= startPos.x)
	{
		return;
	}

	// Check Z- face collision
	if (*velocityZ > 0 && hitbox->endPos.z <= startPos.z)
	{
		float distance = startPos.z - hitbox->endPos.z;
		*velocityZ = distance < *velocityZ ? distance : *velocityZ;
	}
	// Check Z+ face collision
	else if (*velocityZ < 0 && hitbox->startPos.z >= endPos.z)
	{
		float distance = endPos.z - hitbox->startPos.z;
		*velocityZ = distance > *velocityZ ? distance : *velocityZ;
	}
}

bool AABB::intersectLine(glm::vec3 startVec, glm::vec3 endVector, glm::vec3* outIntersect, glm::vec3* outNormal)
{
	float fLow = 0;
	float fHigh = 1;

	//Check line collide onXYZ
	if (!clipLine(startPos.x, endPos.x, startVec.x, endVector.x, &fLow, &fHigh))
	{
		return false;
	}
	if (!clipLine(startPos.y, endPos.y, startVec.y, endVector.y, &fLow, &fHigh))
	{
		return false;
	}
	if (!clipLine(startPos.z, endPos.z, startVec.z, endVector.z, &fLow, &fHigh))
	{
		return false;
	}

	glm::vec3 direction = endVector - startVec;
	*outIntersect = startVec + direction * fLow;
	outIntersect->x = MathUtil::lessDecimal(outIntersect->x, 4);
	outIntersect->y = MathUtil::lessDecimal(outIntersect->y, 4);
	outIntersect->z = MathUtil::lessDecimal(outIntersect->z, 4);

	// Get normal
	if (outIntersect->y == startPos.y)
	{
		*outNormal = glm::vec3(0, -1, 0);
	}
	else if (outIntersect->y == endPos.y)
	{
		*outNormal = glm::vec3(0, 1, 0);
	}
	else if (outIntersect->x == startPos.x)
	{
		*outNormal = glm::vec3(-1, 0, 0);
	}
	else if (outIntersect->x == endPos.x)
	{
		*outNormal = glm::vec3(1, 0, 0);
	}
	else if (outIntersect->z == startPos.z)
	{
		*outNormal = glm::vec3(0, 0, -1);
	}
	else if (outIntersect->z == endPos.z)
	{
		*outNormal = glm::vec3(0, 0, 1);
	}
	else
	{
		*outNormal = glm::vec3(0, 0, 0);
	}

	return true;
}

bool AABB::clipLine(float x1, float x2, float startVecX, float endVectorX, float* fLow, float* fHigh)
{
	float fDimLow = (x1 - startVecX) / (endVectorX - startVecX);
	float fDimHigh = (x2 - startVecX) / (endVectorX - startVecX);

	if (fDimHigh < fDimLow)
	{
		float swap = fDimLow;
		fDimLow = fDimHigh;
		fDimHigh = swap;
	}

	if (fDimHigh < *fLow || fDimLow > *fHigh)
	{
		return false;
	}

	*fLow = max(fDimLow, *fLow);
	*fHigh = min(fDimHigh, *fHigh);

	if (*fLow > *fHigh)
	{
		return false;
	}

	return true;
}