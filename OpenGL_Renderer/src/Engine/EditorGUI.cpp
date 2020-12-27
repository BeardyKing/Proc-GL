#include "EditorGUI.h"

#include "ECS_List.h"


EditorGUI::EditorGUI() {

}

EditorGUI::~EditorGUI() {

}


void EditorGUI::RenderHierarchy() {
	//std::cout << GetAmountOfEntities() << std::endl;
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
	ImGui::Begin("ECS_Hierarchy");
	{
		for (uint32_t i = 0; i < GetAmountOfEntities(); i++) {
			auto label = GetManager()->entities[i]->getComponent<ObjectData>().GetName();
			if (ImGui::Selectable(label, GetManager()->Editor_GetActiveEntity() == i)) {
				GetManager()->Editor_SetActiveEntity(i);
			}
		}
	}

	ImGui::SameLine();
	ImGui::End();
}

void EditorGUI::RenderScene(GLuint& renderTexture) {
    if (G_GetWindowResizeFlag()) {
        UpdateFrameBufferTextureSize(renderTexture);
    }

    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Scene");

    ImVec2 windowSize = ImGui::GetContentRegionAvail();

    if (windowSize.x != lastFrameWindowSize.x || windowSize.y != lastFrameWindowSize.y) {
        lastFrameWindowSize = windowSize;
        windowSizeChangeFlag = true;

    }


    {

        ImTextureID tex = (void*)renderTexture; // Texture from framebuffer

        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 uv_min = ImVec2(0.0f, 0.0f);
        ImVec2 uv_max = ImVec2(1.0f, -1.0f);
        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
        ImGui::Image(tex, windowSize, uv_min, uv_max, tint_col, border_col);


        // ImGuizmo
        Entity* currentEntity = GetManager()->GetSelectedEntity();
        static Entity* cam;

        if (!cam) {
            for (auto& m_entity : GetManager()->entities) {
                if (m_entity->hasComponent<FPSCamera>()) {
                    cam = m_entity.get();
                    break;
                }
            }
        }

        if (currentEntity && cam) {
            ImGui::Begin("Scene");

            std::cout << currentEntity->getComponent<ObjectData>().GetName() << std::endl;
            std::cout << cam->getComponent<ObjectData>().GetName() << std::endl;

            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            float windowWidth = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			glm::mat4 model, view, projection;

			view = cam->getComponent<FPSCamera>().GetViewMatrix();
			
				projection = glm::perspective(cam->getComponent<FPSCamera>().getFOV(), windowWidth / windowHeight, 0.1f, 100.0f);

			////----------------------------------//
			glm::vec3 viewPos = cam->getComponent<FPSCamera>().GetPosition();
			////----------------------------------//

			model = glm::mat4(1.0f);
			model =
				glm::translate(model, cam->getComponent<Transform>().position) *
				glm::rotate(model, glm::radians(cam->getComponent<Transform>().rotation.x), glm::vec3(1, 0, 0)) *
				glm::rotate(model, glm::radians(cam->getComponent<Transform>().rotation.y), glm::vec3(0, 1, 0)) *
				glm::rotate(model, glm::radians(cam->getComponent<Transform>().rotation.z), glm::vec3(0, 0, 1)) *
				glm::scale(model, cam->getComponent<Transform>().scale);

            ////entity
            glm::mat4 target = currentEntity->getComponent<Transform>().GetTransform();

            ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, glm::value_ptr(target));

            ImGui::End();
        }



        ImGui::End();
    }
}

void EditorGUI::RenderInspector() {

}

void EditorGUI::RenderProject() {
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Project");
    ImGui::End();
}

void EditorGUI::RenderConsole() {
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

    ImGui::Begin("Console");

    ImGui::Columns(2, "word-wrapping", true);
    ImGui::SetColumnOffset(1, 50);
    ImGui::Separator();
    ImGui::TextWrapped("LOG");
    ImGui::NextColumn();
    ImGui::TextWrapped("testing log ");
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Columns(2, "word-wrapping", true);
    ImGui::SetColumnOffset(1, 50);
    ImGui::Separator();
    ImGui::TextWrapped("ERR");
    ImGui::NextColumn();
    ImGui::TextWrapped("testing err ");
    ImGui::Columns(2);
    ImGui::Separator();

    ImGui::End();
}

void EditorGUI::RenderMainMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("ITEM", "CTRL+O")) {
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("ITEM", "ACTION")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void EditorGUI::UpdateFrameBufferTextureSize(GLuint renderTexture) {
    glBindTexture(GL_TEXTURE_2D, renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, lastFrameWindowSize.x, lastFrameWindowSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    windowSizeChangeFlag = false;
    glViewport(0, 0, lastFrameWindowSize.x, lastFrameWindowSize.y);

    //GetComponent FPSCamera
    for (auto& entity : GetManager()->entities) {
        if (entity->hasComponent<FPSCamera>()) {
            auto& cam = entity->getComponent<FPSCamera>();
            cam.ImGuiWindowSize = lastFrameWindowSize;
        }
    }
}

void EditorGUI::RenderActiveInspector() {
    GetManager()->Editor_RenderActiveInspector();
}

void  EditorGUI::Demo(bool &demo) {


    if (&demo)
        ImGui::ShowDemoWindow(&demo);
}