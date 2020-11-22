#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/gtc/type_ptr.hpp>
#include "../ECS/Component.h"
#include <iostream>
#include "../vendor/imgui/imgui.h"

	class Transform : public Component
	{
	public:
		Transform() :
			position(glm::vec3(0)),
			rotation(glm::vec3(0)),
			scale(glm::vec3(1))
		{}


		Transform(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale) {
			position = _position;
			rotation = _rotation;
			scale = _scale;
		}

		void OnRender() {}

		void OnUpdate(double deltaTime) {}

		bool ShowTransform = true;

		void OnImGuiRender() {
			
			ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
			ImGui::Begin("Inspector");
			{
				ImGui::Separator();
				ImGui::Text("Transform");

				ImGui::Text("Position : "); ImGui::SameLine();
				ImGui::DragFloat3("P", &position.x, -0.1f, 0.1f);

				ImGui::Text("Rotation : "); ImGui::SameLine();
				ImGui::DragFloat3("R", &rotation.x, -1.0f, 1.0f);

				ImGui::Text("Scale :    "); ImGui::SameLine();
				ImGui::DragFloat3("S", &scale.x, -0.1f, 0.1f);

				ImGui::Separator();
			}
			ImGui::End();
		}

		~Transform() = default;

	public:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};




#endif // !TRANSFORM_H



