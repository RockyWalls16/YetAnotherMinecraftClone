#include <math/AABB.h>
#include <core/block/Block.h>
#include <core/world/World.h>
#include <cmath>

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

std::vector<AABB*>* AABB::tilesInBox(World* world)
{
	std::vector<AABB*>* aabbs = new std::vector<AABB*>();
	int x, y, z;
	int bX = floor(startPos.x);
	int bY = floor(startPos.y);
	int bZ = floor(startPos.z);
	int bX2 = ceil(endPos.x);
	int bY2 = ceil(endPos.y);
	int bZ2 = ceil(endPos.z);
	Block* block;

	for (y = bY; y < bY2; y++)
	{
		for (x = bX; x < bX2; x++)
		{
			for (z = bZ; z < bZ2; z++)
			{
				block = world->getBlockAt(x, y, z);
				AABB* aabb = block->getHitbox(x, y, z);

				if (aabb != nullptr)
				{
					aabbs->push_back(aabb);
				}
			}
		}
	}

	return aabbs;
}

AABB * AABB::expandBox(glm::vec3 expandAABB)
{
	AABB* newBox = new AABB(startPos, endPos);

	// Expand X
	if (expandAABB.x < 0)
	{
		newBox->startPos.x += expandAABB.x;
	}
	else
	{
		newBox->endPos.x += expandAABB.x;
	}

	// Expand Y
	if (expandAABB.y < 0)
	{
		newBox->startPos.y += expandAABB.y;
	}
	else
	{
		newBox->endPos.y += expandAABB.y;
	}

	// Expand Z
	if (expandAABB.z < 0)
	{
		newBox->startPos.z += expandAABB.z;
	}
	else
	{
		newBox->endPos.z += expandAABB.z;
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
