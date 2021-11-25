#include "script_simpleBehaviours.h"
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/matrix_decompose.hpp>

script_simplebehaviours::script_simplebehaviours() {}
script_simplebehaviours::~script_simplebehaviours() {
	std::cout << "Deleted - script_simplebehaviours" << std::endl;
}
void script_simplebehaviours::OnRender() {}
void script_simplebehaviours::OnUpdate(double deltaTime) {

	if (rotate){
		entity->getComponent<Transform>().rotation = 
			entity->getComponent<Transform>().rotation + 
			glm::vec3(
				rotationSpeed * deltaTime * rotateX,
				rotationSpeed * deltaTime * rotateY,
				rotationSpeed * deltaTime * rotateZ
			);
	}

	if (orbit){
		m_movingLightAngle += (float)deltaTime * 90.0f;	// rotate lights

		entity->getComponent<Transform>().position.x = 1.5f + 10 * sinf(glm::radians(m_movingLightAngle));
		entity->getComponent<Transform>().position.z = 1.5f + 10 * cosf(glm::radians(m_movingLightAngle));
		entity->getComponent<Transform>().position.y = 8 + (0.5f * sinf(glm::radians(m_movingLightAngle) * 4));
	}
	if (ImGui::IsKeyReleased('E')) { isFPSCam = !isFPSCam; }

	if (isFPSCam == true){
		if (entity->hasComponent<FPSCamera>()){
			entity->getComponent<FPSCamera>().isLookingAtTargetPosition = false;
		}
	}
	if (isMovingOnCameraPath == true && isFPSCam == false) {
		IsMovingOnCameraPath(deltaTime);
	}
}

glm::vec3 lerp(glm::vec3 x, glm::vec3 y, float t) {
	return x * (1.f - t) + y * t;
}

std::vector<glm::vec3> targetLookAt
{
	/*glm::vec3(0,0,0),
	glm::vec3(0,0,0),
	glm::vec3(0,0,0),
	glm::vec3(0,0,0),
	glm::vec3(0,0,0),
	glm::vec3(0,0,-40),
	glm::vec3(0,0,-44),
	glm::vec3(0,0,-16),
	glm::vec3(0,0,-7),
	glm::vec3(0,0,0),
	glm::vec3(0,0,-35),
	glm::vec3(0,-3,-16),
	glm::vec3(-6,-7,1),
	glm::vec3(-9,-10,-3),
	glm::vec3(43,-44,131),
	glm::vec3(-200,-75,6),
	glm::vec3(14,4,79),
	glm::vec3(47,-26,106),
	glm::vec3(-111,-13,-1),
	glm::vec3(30,-23,1),
	glm::vec3(5,-14,11),
	glm::vec3(-4,-19,1)*/
	glm::vec3(0),
	glm::vec3(0),
	glm::vec3(0),
	glm::vec3(0),
	glm::vec3(0),
	glm::vec3(0),
	glm::vec3(0),
	glm::vec3(0),
	glm::vec3(0),
	glm::vec3(0),
	glm::vec3(0),
	glm::vec3(0),
	glm::vec3(0),
	glm::vec3(0),
	glm::vec3(0),
	glm::vec3(0),
	glm::vec3(0),
	glm::vec3(0),
	glm::vec3(0),
	glm::vec3(0),
	glm::vec3(0),
	glm::vec3(0)
};

std::vector<glm::vec3> targetPositions
{
	glm::vec3(73 ,4  ,24),
	glm::vec3(67 ,4  ,64),
	glm::vec3(45 ,4  ,75),
	glm::vec3(25 ,2  ,60),
	glm::vec3(19 ,-5 ,41),
	glm::vec3(18 ,-5 ,20),
	glm::vec3(36 ,2  ,8),
	glm::vec3(48 ,3  ,-10),
	glm::vec3(43 ,0  ,-45),
	glm::vec3(28 ,0  ,-62),
	glm::vec3(-11,0  ,-61),
	glm::vec3(-32 ,0 ,-51),
	glm::vec3(-36 ,-6 ,-27),
	glm::vec3(-42 ,-7 ,12),
	glm::vec3(-47 ,-13  ,40),
	glm::vec3(-61 ,-15  ,49),
	glm::vec3(-92 ,-18  ,27),
	glm::vec3(-101 ,0  ,17),
	glm::vec3(-96 ,6  ,-40),
	glm::vec3(-74 ,4  ,-89),
	glm::vec3(12 ,7  ,-113),
	glm::vec3(71 ,8  ,-51)
};

int currentTargetPos = 0;
float next_distance = 1.0f;
double speed = 30.0;
bool setPos = true;
bool isActive;
int debug_CurrentPos = 0;

void script_simplebehaviours::IsMovingOnCameraPath(double deltaTime){
	if (setPos){
		setPos = false;
		entity->getComponent<FPSCamera>().SetPosition(targetPositions[0]);
	}

	glm::vec3 currentPos = entity->getComponent<FPSCamera>().GetPosition();
	glm::vec3 targetPos = targetPositions[currentTargetPos];
	glm::vec3 targetLookAtPos = targetLookAt[currentTargetPos];
	timer += deltaTime;
	double pitch = 0.0;
	if (glm::distance(currentPos, targetPos) < next_distance){
		if (currentTargetPos < targetPositions.size() - 1){
			currentTargetPos++;
			timer = 0;
		}
		else{
			currentTargetPos = 0;
		}
	}

	entity->getComponent<FPSCamera>().isLookingAtTargetPosition = true;
	entity->getComponent<FPSCamera>().inputTargetPosition = targetLookAt[0];
	
	glm::vec3 moveDir = targetPos - currentPos;
	if (moveDir == glm::vec3(0)) {
		std::cout << "ERR" << std::endl;
		moveDir = glm::vec3(1);
	}
	glm::vec3 normDir = glm::normalize(moveDir);

	//moveDir = glm::normalize(moveDir);
	std::cout << "NORM" << std::endl;
	std::cout << normDir.x << std::endl;
	std::cout << normDir.y << std::endl;
	std::cout << normDir.z << std::endl;

	std::cout << "DIR" << std::endl;
	std::cout << moveDir.x << std::endl;
	std::cout << moveDir.y << std::endl;
	std::cout << moveDir.z << std::endl;
	currentPos = currentPos + (glm::normalize(moveDir) * (float)deltaTime * 9.0f);
	entity->getComponent<FPSCamera>().SetPosition(currentPos);
	//entity->getComponent<FPSCamera>().SetPosition(lerp(currentPos, targetPos, deltaTime));


}

void script_simplebehaviours::OnImGuiRender() {
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

	ImGui::Begin("Inspector"); {
		ImGui::Separator();
		ImGui::Text("NODE POSITION DEBUG");
		std::string timer_val = "TIMER : ";
		timer_val.append(std::to_string(timer));
		ImGui::Text(timer_val.c_str());
		ImGui::InputInt("DEBUG currentPos ", &debug_CurrentPos, 1);
		if (debug_CurrentPos > targetPositions.size()) {
			debug_CurrentPos = targetPositions.size();
		}

		ImGui::Text("NODE POSITION DEBUG");
		ImGui::Text("NODE LOOK AT POS");
		int j = 0;
		for (auto& v : targetLookAt) {
			j++;
			std::string name = "targetLookAt : ";
			name.append(std::to_string(j));
			ImGui::DragFloat3(name.c_str(), &v.x);
		}
		ImGui::Separator();

		ImGui::Text("NODE POSITIONS");
		int i = 0;
		for (auto& v : targetPositions){
			i++;
			std::string name = "NodePos : ";
			name.append(std::to_string(i));
			ImGui::DragFloat3(name.c_str(), & v.x);
		}
		ImGui::Separator();


		




		ImGui::Separator();
		ImGui::DragFloat3("test_rotation", &test_rotation.x);
		if (ImGui::CollapsingHeader("Behaviour", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
			ImGui::Indent();
				ImGui::Checkbox("rotate", &rotate);
				ImGui::Checkbox("orbit", &orbit);
				if (rotate){
					ImGui::DragFloat("RotationSpeed", &rotationSpeed);
					ImGui::Checkbox("rotate X", &rotateX);
					ImGui::Checkbox("rotate Y", &rotateY);
					ImGui::Checkbox("rotate Z", &rotateZ);
				}
			ImGui::Unindent();
		}
	}
	ImGui::End();
}

#pragma region Orbit_Functions

void script_simplebehaviours::SetOrbitActive(bool b) {
	orbit = b;
}

void script_simplebehaviours::SetCameraMoveActive(bool b) {
	isMovingOnCameraPath = b;
}


#pragma endregion


#pragma region Rotation_Functions

void script_simplebehaviours::SetRotateActive(bool b) {
	rotate = b;
}

void script_simplebehaviours::SetRotateAxis(bool x, bool y, bool z) {
	rotateX = x;
	rotateY = y;
	rotateZ = z;
}

void script_simplebehaviours::SetRotationSpeed(float speed) {
	rotationSpeed = speed;
}

#pragma endregion

void script_simplebehaviours::OnExit() {

}