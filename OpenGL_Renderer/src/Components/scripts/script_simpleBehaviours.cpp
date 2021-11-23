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
	if (isMovingOnCameraPath == true) {
		IsMovingOnCameraPath(deltaTime);
	}
}

glm::vec3 lerp(glm::vec3 x, glm::vec3 y, float t) {
	return x * (1.f - t) + y * t;
}

std::vector<glm::vec3> targetLookAt
{
	glm::vec3(0),
	glm::vec3(4),
	glm::vec3(-4)
};

std::vector<glm::vec3> targetPositions
{
	glm::vec3(-40,10,10),
	glm::vec3(40,10,10),
	glm::vec3(10,10,40)
};

int currentTargetPos = 0;
float next_distance = 1.0f;
double speed = 30.0;
float timer = 0;

void script_simplebehaviours::IsMovingOnCameraPath(double deltaTime){
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
	//entity->getComponent<FPSCamera>().inputTargetPosition = targetLookAt[currentTargetPos];
	glm::vec3 nextLookat = targetLookAt[0];
	if (currentTargetPos < targetPositions.size()) {
		nextLookat = targetLookAt[currentTargetPos + 1];
	}
	glm::vec3 tmp_lookat_target = lerp(targetLookAt[currentTargetPos], targetLookAt[currentTargetPos + 1], timer);
	entity->getComponent<FPSCamera>().inputTargetPosition =  targetLookAt[currentTargetPos];
	entity->getComponent<FPSCamera>().SetPosition(lerp(currentPos, targetPos, deltaTime));

}


void script_simplebehaviours::OnImGuiRender() {
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

	ImGui::Begin("Inspector"); {

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