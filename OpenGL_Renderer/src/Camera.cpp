#include "Camera.h"
#include "glm/gtx/transform.hpp"

const float DEFAULT_FOV = 45.0f;

// --------------------------------
// CAMERA BASE CLASS
// --------------------------------

Camera::Camera() 
	:mPosition(glm::vec3(0.0f, 0.0f, 0.0f)), 
	mTarget(glm::vec3(0.0f, 0.0f, 0.0f)), 
	mUp(glm::vec3(0.0f, 0.1f, 0.0f)),
	mRight(0.0f, 0.0f, 0.0f),
	WORLD_UP(0.0f, 1.0f, 0.0f),
	mYaw(glm::pi<float>()),
	mPitch(0.0f),
	mRoll(0.0f),
	mFOV(DEFAULT_FOV)
{
}

glm::mat4 Camera::GetViewMatrix()const {
	return glm::lookAt(mPosition, mTarget, mUp);
}

const glm::vec3& Camera::GetLook()	const { //forward
	return mLook;
}

const glm::vec3& Camera::GetRight()	const { 
	return mRight;
}

const glm::vec3& Camera::GetUp()	const { 
	return mUp;
}

const glm::vec3 Camera::GetPosition()const {
	return mPosition;
}


// --------------------------------
// FPS CAMERA
// --------------------------------

FPSCamera::FPSCamera(glm::vec3 position, float yaw, float pitch) {
	mPosition = position;
	mYaw = yaw;
	mPitch = pitch;
}

void FPSCamera::SetPosition(const glm::vec3& position){
	mPosition = position;
}

void FPSCamera::Move(const glm::vec3& offsetPos) {
	mPosition += offsetPos;
	UpdateCameraVectors();
}

void FPSCamera::Rotate(float yaw, float pitch) {
	mYaw += glm::radians(yaw);
	mPitch += glm::radians(pitch);

	// constrain pitch
	mPitch = glm::clamp(mPitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);
	UpdateCameraVectors();
}

void FPSCamera::UpdateCameraVectors() {

	//spherical to cartesian coords // y is up not z

	glm::vec3 look;
	look.x = cosf(mPitch) * sinf(mYaw);
	look.y = sinf(mPitch);
	look.z = cosf(mPitch) * cosf(mYaw);

	mLook = glm::normalize(look);

	//recalc right & up vector
	mRight = glm::normalize(glm::cross(mLook, WORLD_UP));
	mUp = glm::normalize(glm::cross(mRight, mLook));

	mTarget = mPosition + mLook;
}

// --------------------------------
// ORBIT CAMERA
// --------------------------------

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