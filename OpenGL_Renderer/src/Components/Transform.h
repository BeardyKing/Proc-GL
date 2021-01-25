#ifndef TRANSFORM_H
#define TRANSFORM_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/transform.hpp"


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

		glm::mat4 GetTransformMatrix() {
			glm::mat4 matRotation = glm::toMat4(glm::quat(glm::radians(rotation)));
			glm::mat4 outVal = glm::mat4(1.0f);
			outVal =
				glm::translate(glm::mat4(1.0f), position) *
				matRotation *
				glm::scale(glm::mat4(1.0f), scale);
			return outVal;
		}

		void OnRender() {}

		void OnUpdate(double deltaTime) {}

		bool ShowTransform = true;
		bool flag = true;
		void OnImGuiRender() {
			ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
			ImGui::Begin("Inspector");
			{
				ImGui::Separator();
				if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
					ImGui::Text("Transform");

					ImGui::Text("Position : "); ImGui::SameLine();
					ImGui::DragFloat3("P", &position.x, -0.1f, 0.1f);

					ImGui::Text("Rotation : "); ImGui::SameLine();
					ImGui::DragFloat3("R", &rotation.x, -1.0f, 1.0f);

					ImGui::Text("Scale :    "); ImGui::SameLine();
					ImGui::DragFloat3("S", &scale.x, -0.1f, 0.1f);
				}
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



