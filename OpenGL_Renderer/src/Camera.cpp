#include "Camera.h"
#include "glm/gtx/transform.hpp"

Camera::Camera() 
	:mPosition(glm::vec3(0.0f, 0.0f, 0.0f)), 
	mTarget(glm::vec3(0.0f, 0.0f, 0.0f)), 
	mUp(glm::vec3(0.0f, 0.1f, 0.0f)),
	mYaw(0),
	mPitch(0),
	mRoll(0)
{
}

glm::mat4 Camera::getViewMatrix()const {
	return glm::lookAt(mPosition, mTarget, mUp);
}


//Orbit Camera

OrbitCamera::OrbitCamera() 
	:mRadius(10.0f)
{
}

void OrbitCamera::SetLookAt(const glm::vec3& target) {
	mTarget = target;
}

void OrbitCamera::SetRadius(float radius) {
	mRadius = glm::clamp(radius, 2.0f, 80.0f);
}

void OrbitCamera::Rotate(float yaw, float pitch) {
	mYaw = glm::radians(yaw);
	mPitch = glm::radians(pitch);

	mPitch = glm::clamp(mPitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);

	UpdateCameraVectors();
}

void OrbitCamera::UpdateCameraVectors() {
	mPosition.x = mTarget.x + mRadius * cosf(mPitch) * sinf(mYaw);
	mPosition.y = mTarget.y + mRadius * sinf(mPitch);
	mPosition.z = mTarget.z + mRadius * cosf(mPitch) * cosf(mYaw);
}