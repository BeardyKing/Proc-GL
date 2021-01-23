#include "script_simpleBehaviours.h"

script_simplebehaviours::script_simplebehaviours() {}
script_simplebehaviours::~script_simplebehaviours() {}

void script_simplebehaviours::OnRender() {

}
void script_simplebehaviours::OnUpdate(double deltaTime) {
	
	entity->getComponent<Transform>().rotation = entity->getComponent<Transform>().rotation + glm::vec3(0, 180 * deltaTime, 0);
}
void script_simplebehaviours::OnImGuiRender() {
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

	ImGui::Begin("Inspector"); {
		ImGui::Separator();

		if (ImGui::CollapsingHeader("simple behaviour", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {}
		
		ImGui::Separator();
	}
	ImGui::End();
}
void script_simplebehaviours::OnExit() {

}