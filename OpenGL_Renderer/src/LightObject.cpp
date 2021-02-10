#include "LightObject.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"

LightObject::LightObject() :
	lightType(LightType::Point),
	color(glm::vec3(300.0f, 150.0f, 150.0f))
{}

LightObject::~LightObject() {}
void LightObject::OnRender(){}
void LightObject::OnUpdate(double deltaTime){}

void LightObject::OnImGuiRender(){
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

	ImGui::Begin("Inspector");{
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Light Object", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
			ImGui::ColorPicker3("Light Color", &color.r);
		}

		ImGui::Separator();
	}
	ImGui::End();
}

