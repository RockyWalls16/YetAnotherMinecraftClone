/*
 * MathUtil.h
 *
 *  Created on: 7 sept. 2017
 *      Author: Valentin
 */

#ifndef MATH_MATHUTIL_H_
#define MATH_MATHUTIL_H_

#include <glm/vec3.hpp>

#define M_PI 3.14159265358979323846

class MathUtil
{
public:
	static glm::vec3 lerpVec3(const glm::vec3& start, const glm::vec3& end, float amount);

	static glm::vec3 getForwardVector(const glm::vec3& direction);

	static glm::vec3 getFlatForwardVector(const glm::vec3& direction);

	static glm::vec3 getFlatStrafingVector(const glm::vec3& direction);

	static float lessDecimal(float value, float decimal);

	static float lerp(float start, float end, float time);

	static int getChunkTilePosFromWorld(int pos);

	static int rand(int min, int max);

	static float randF();

	static float randF(float min, float max);
};

#endif /* UTIL_MATHUTIL_H_ */
