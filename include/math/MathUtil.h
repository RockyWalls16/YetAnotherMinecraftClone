/*
 * MathUtil.h
 *
 *  Created on: 7 sept. 2017
 *      Author: Valentin
 */

#ifndef MATH_MATHUTIL_H_
#define MATH_MATHUTIL_H_

#include <glm/vec3.hpp>

class MathUtil
{
public:
	static glm::vec3 lerpVec3(const glm::vec3& start, const glm::vec3& end, float amount);

	static glm::vec3 getForwardVector(const glm::vec3& direction);

	static glm::vec3 getFlatForwardVector(const glm::vec3& direction);

	static glm::vec3 getFlatStrafingVector(const glm::vec3& direction);
};

#endif /* UTIL_MATHUTIL_H_ */
