#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"

//---
//Abstract Camera Class
//---

class Camera {
public:

	glm::mat4 getViewMatrix()const;
	virtual void Rotate(float yaw, float pitch) {} // float roll // in degrees


protected:
	Camera();

	glm::vec3 mPosition;
	glm::vec3 mTarget;
	glm::vec3 mUp;


	//Euler angle (in radians)
	float mYaw;
	float mPitch;
	float mRoll;

};

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