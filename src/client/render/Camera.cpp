/*
 * Camera.cpp
 *
 *  Created on: 31 ao�t 2017
 *      Author: Valentin
 */

#include <client/render/Camera.h>

#include <math/MathUtil.h>
#include <util/TimeManager.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <util/Logger.h>
#include <string>
#include <math/Frustum.h>

void Camera::setCameraPerspective(float fov, int width, int height)
{
	this->fov = fov;
	this->aspect = (float)width / (float)height;
	projectionMatrix = glm::perspective(glm::radians(fov), aspect, NEAR_PLANE, FAR_PLANE);
	viewProjMatrix = projectionMatrix * viewMatrix;

	Frustum::updateFrustumSize(this);
}

void Camera::updateCameraRender()
{
	static glm::vec3 xAxis = glm::vec3(-1.0F, 0.0F, 0.0F);
	static glm::vec3 yAxis = glm::vec3(0.0F, 1.0F, 0.0F);

	if(trackedEntity)
	{
		location = MathUtil::lerpVec3(trackedEntity->getLastPosition(), trackedEntity->getPosition(), TimeManager::getTickInterpolation());
		location.y += trackedEntity->getEyeHeight();

		rotation = MathUtil::lerpVec3(trackedEntity->getLastRotation(), trackedEntity->getRotation(), TimeManager::getTickInterpolation());
	}

	// Calculate matrices
	viewMatrix = glm::mat4(1.0F);
	viewMatrix = glm::rotate(viewMatrix, glm::radians(rotation.x), xAxis);
	viewMatrix = glm::rotate(viewMatrix, glm::radians(rotation.y), yAxis);
	viewMatrix = glm::translate(viewMatrix, glm::vec3(-location.x, -location.y, -location.z));
	viewProjMatrix = projectionMatrix * viewMatrix;

	Frustum::computePlanes(this);
}

void Camera::updateVectors()
{
	flatForwardVector = MathUtil::getFlatForwardVector(rotation);
	forwardVector = MathUtil::getForwardVector(rotation);
	strafeVector = MathUtil::getFlatStrafingVector(rotation);
	upVector = glm::cross(strafeVector, forwardVector);
}

void Camera::move(const glm::vec3& velocity)
{
	if(trackedEntity)
	{
		trackedEntity->setControllerVelocity(velocity);
	}
	else
	{
		location += velocity * TimeManager::getFDelta() * 10.0F;
	}
}

void Camera::inputMove(glm::vec3 inputVelocity)
{
	if (trackedEntity)
	{
		glm::vec3 movement = glm::vec3(flatForwardVector * inputVelocity.x + strafeVector * inputVelocity.z);
		movement.y = inputVelocity.y;
		move(movement);
	}
	else
	{
		glm::vec3 movement = glm::vec3(forwardVector * inputVelocity.x + strafeVector * inputVelocity.z);
		movement.y = inputVelocity.y;
		move(movement);
	}
}

void Camera::moveForwardVector(float value)
{
	if (trackedEntity)
	{
		move(flatForwardVector * value);
	}
	else
	{
		move(forwardVector * value);
	}
}

void Camera::moveStrafingVector(float value)
{
	move(strafeVector * value);
}

void Camera::moveUpVector(float value)
{
	glm::vec3 velocity;
	velocity.y = value;
	move(velocity);
}

void Camera::setRotation(const glm::vec3& nRotation)
{
	glm::vec3 newRotation = nRotation;

	// Clamp rotation
	if(nRotation.x >= 89.5)
	{
		newRotation.x = 89.5;
	}
	else if(nRotation.x <= -89.5)
	{
		newRotation.x = -89.5;
	}

	if(trackedEntity)
	{
		trackedEntity->setRotation(newRotation, true);
	}
	else
	{
		rotation = newRotation;
	}

	updateVectors();
}

const glm::mat4& Camera::getProjectionMatrix() const
{
	return projectionMatrix;
}

const glm::mat4& Camera::getViewMatrix() const
{
	return viewMatrix;
}

const glm::mat4& Camera::getViewProjMatrix() const
{
	return viewProjMatrix;
}

const glm::vec3& Camera::getLocation() const
{
	return location;
}

const glm::vec3& Camera::getRotation() const
{
	return rotation;
}

const glm::vec3 & Camera::getForwardVec() const
{
	return forwardVector;
}

const glm::vec3 & Camera::getStrafeVec() const
{
	return strafeVector;
}

const glm::vec3 & Camera::getUpVec() const
{
	return upVector;
}

const float Camera::getFov() const
{
	return fov;
}

const float Camera::getAspect() const
{
	return aspect;
}

bool Camera::canRepeatJump()
{
	return trackedEntity == nullptr;
}

Entity* Camera::getTrackedEntity()
{
	return trackedEntity;
}

void Camera::setTrackedEntity(Entity& trackedEntity)
{
	this->trackedEntity = &trackedEntity;
}