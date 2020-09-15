#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

//---
//Abstract Camera Class
//---

class Camera {
public:

	glm::mat4 GetViewMatrix()const;

	virtual void SetPosition(const glm::vec3& position)  {}
	virtual void Rotate		(float yaw, float pitch)	 {} // float roll // in degrees
	virtual void Move		(const glm::vec3& offsetPos) {}

	const glm::vec3& GetLook()	const;
	const glm::vec3& GetRight() const;
	const glm::vec3& GetUp()	const;

	float getFOV() const	{ return mFOV; }
	void  setFOV(float fov) { mFOV = fov; } // in Degrees

protected:
	Camera();

	virtual void UpdateCameraVectors() {}

	glm::vec3 mPosition;
	glm::vec3 mTarget;
	glm::vec3 mUp;
	glm::vec3 mLook; // forwards
	glm::vec3 mRight;
	const glm::vec3 WORLD_UP;

	//Euler angle (in radians)
	float mYaw;
	float mPitch;
	float mRoll;

	float mFOV; //degrees

};
//--------------------
class FPSCamera : public Camera {
public:
	FPSCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = glm::pi<float>(), float pitch = 0.0f); // initial angle faces z 
	virtual void SetPosition(const glm::vec3& position);
	virtual void Rotate		(float yaw, float pitch);		// in degrees
	virtual void Move		(const glm::vec3& offsetPos);

private:

	void UpdateCameraVectors();
};

//--------------------
class OrbitCamera : public Camera {
public:
	OrbitCamera();

	virtual void Rotate(float yaw, float pitch); // float roll // in degrees

	void SetLookAt(const glm::vec3& target); 
	void SetRadius(float radius);

private:
	void UpdateCameraVectors();

	float mRadius;

};


#endif