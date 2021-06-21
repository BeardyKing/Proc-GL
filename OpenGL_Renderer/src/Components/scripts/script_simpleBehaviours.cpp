#include "script_simpleBehaviours.h"

script_simplebehaviours::script_simplebehaviours() {}
script_simplebehaviours::~script_simplebehaviours() {}

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
		entity->getComponent<Transform>().position.y = 3 + (0.5f * sinf(glm::radians(m_movingLightAngle) * 4));
	}
}
void script_simplebehaviours::OnImGuiRender() {
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

	ImGui::Begin("Inspector"); {
		ImGui::Separator();
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