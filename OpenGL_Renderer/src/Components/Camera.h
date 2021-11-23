#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

#include "../ECS/Engine_ECS.h"

//---
//Abstract Camera Class
//---

class Camera : public Component{
public:

	glm::mat4 GetViewMatrix()const;

	virtual void SetPosition(const glm::vec3& position)  {}
	virtual void Rotate		(float yaw, float pitch)	 {} // float roll // in degrees
	virtual void Move		(const glm::vec3& offsetPos) {}

	const glm::vec3& GetForward()	const;
	const glm::vec3& GetRight() const;
	const glm::vec3& GetUp()	const;
	const glm::vec3 GetPosition() const;

	float getFOV() const	{ return mFOV; }
	void  setFOV(float fov) { mFOV = fov; } // in Degrees

	void SetZNear(float z) { zNear = z; }
	void SetZFar(float z) { zFar = z; }

	float GetZNear() { return zNear; }
	float GetZFar() { return zFar; }

	void SetTarget(glm::vec3 in_target) { mTarget = in_target; };
	void SetLook(glm::vec3 in_target) { 
		mLook = in_target; 
		mTarget = entity->getComponent<Transform>().position + mLook;
	};

	bool isLookingAtTargetPosition = false;
	glm::vec3 inputTargetPosition = glm::vec3(0);

	glm::vec2 m_mouseSpeedDelta = glm::vec2(-2,2);

protected:
	Camera();

	virtual void UpdateCameraVectors() {}
	Transform* ptr = nullptr;
	glm::vec3 mPosition;
	glm::vec3 mTarget;
	glm::vec3 mUp;
	glm::vec3 mLook; // forwards
	glm::vec3 mRight;
	const glm::vec3 WORLD_UP;

	//Euler angle (in radians)
	float mYaw;
	float mPitch;
	float mRoll; // TODO ADD ROLL TO CAMERA ON Q & E

	float mFOV; //degrees
	
	float m_MoveSpeed = 5;
	float m_MoveSpeedDelta = 4;

	float zNear { 0.1f };
	float zFar{ 800.0f };


};
//--------------------
class FPSCamera : public Camera {
public:
	FPSCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0), float yaw = 0, float pitch = 0.0f); // initial angle faces z 
	virtual void SetPosition(const glm::vec3& position);
	virtual void Rotate		(float yaw, float pitch);		// in degrees
	virtual void Move		(const glm::vec3& offsetPos);

	virtual void OnUpdate(double deltaTime);
	virtual void OnImGuiRender();
	virtual bool init();

	void SetRotate(float yaw, float pitch);
	void SetLookAt(const glm::vec3& target);

	glm::vec3 GetRotation();

	inline virtual ImVec2 GetWindowSize() {
		return ImGuiWindowSize; 
	}

	bool usingImGuiWindow = true;
	ImVec2 ImGuiWindowSize;

private:
	glm::vec2 m_lastMousePos;
	bool m_mouseEnabled = true;
	bool m_mouseFlag = false;

	bool _cameraFocusFlag = false;
	bool w, a, s, d, r, f, l_shift;



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