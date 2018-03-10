/*
 * Camera.h
 *
 *  Created on: 31 août 2017
 *      Author: Valentin
 */

#ifndef CLIENT_RENDER_CAMERA_H_
#define CLIENT_RENDER_CAMERA_H_

#include <core/entity/Entity.h>
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <client/input/CameraRay.h>

#define NEAR_PLANE 0.1F
#define FAR_PLANE 1000.0F


class Camera
{
private:
	glm::vec3 location;
	glm::vec3 rotation;
	glm::vec3 flatForwardVector;
	glm::vec3 forwardVector;
	glm::vec3 strafeVector;
	glm::vec3 upVector;
	float fov;
	float aspect;

	Entity* trackedEntity;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 viewProjMatrix;

	CameraRay cameraRay;

public:
	Camera();
	void initCamera();

	float yaw;
	float pitch;

	void setCameraPerspective(float fov, int width, int height);
	void updateCameraRender();
	void updateVectors();


	void moveForwardVector(float value);
	void moveStrafingVector(float value);
	void moveUpVector(float value);
	void move(const glm::vec3& velocity);
	void inputMove(glm::vec3 inputVelocity);

	const glm::mat4& getProjectionMatrix() const;
	const glm::mat4& getViewMatrix() const;
	const glm::mat4& getViewProjMatrix() const;
	const glm::vec3& getLocation() const;
	const glm::vec3& getRotation() const;
	const glm::vec3& getForwardVec() const;
	const glm::vec3& getStrafeVec() const;
	const glm::vec3& getUpVec() const;
	const float getFov() const;
	const float getAspect() const;
	bool canRepeatJump();
	Entity* getTrackedEntity();
	void setTrackedEntity(Entity& trackedEntity);
	void setRotation(const glm::vec3& nRotation);

	CameraRay& getCameraRay();
};

#endif /* CLIENT_RENDER_CAMERA_H_ */
