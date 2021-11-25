#ifndef SCRIPT_SIMPLEBEHAVIOURS
#define SCRIPT_SIMPLEBEHAVIOURS

#include "../../ECS/Component.h"
#include "../../Engine/ECS_List.h"
#include "../../vendor/imgui/imgui.h"

//#include <memory>
#include <iostream>
#include <string>


class script_simplebehaviours : public Component
{
public:
	script_simplebehaviours();

	~script_simplebehaviours();

	void OnRender();
	void OnUpdate(double deltaTime);
	void OnImGuiRender();
	void OnExit();

	void SetRotateActive(bool b);
	void SetRotateAxis(bool x, bool y, bool z);
	void SetRotationSpeed(float speed);

	void SetOrbitActive(bool b);
	void SetCameraMoveActive(bool b);
	void IsMovingOnCameraPath(double deltaTime);

private:
		bool rotate = false;
		float rotationSpeed = 90;
		bool rotateX = false;
		bool rotateY = false;
		bool rotateZ = false;

		bool orbit = false;
		bool isMovingOnCameraPath = false;
		float m_movingLightAngle = 90;
		glm::vec3 centre;
		float timer = 0;
		bool isFPSCam = false;


		glm::vec3 test_rotation = glm::vec3(0);



};




#endif // !SCRIPT_SIMPLEBEHAVIOURS
