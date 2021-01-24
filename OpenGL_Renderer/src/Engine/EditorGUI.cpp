#include "EditorGUI.h"

#include "ECS_List.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/detail/setup.hpp>
#include <glm/gtc/matrix_transform.hpp>


EditorGUI::EditorGUI() {

}

EditorGUI::~EditorGUI() {

}

void EditorGUI::OnGizmoRender() {}

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

			glm::mat4 view, projection;

			view = cam->getComponent<FPSCamera>().GetViewMatrix();
			projection = glm::perspective(cam->getComponent<FPSCamera>().getFOV(), windowWidth / windowHeight, 0.1f, 100.0f);
            static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
            static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);

            if (ImGui::IsKeyPressed(49))
                mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
            if (ImGui::IsKeyPressed(50))
                mCurrentGizmoOperation = ImGuizmo::ROTATE;
            if (ImGui::IsKeyPressed(51)) // r Key
                mCurrentGizmoOperation = ImGuizmo::SCALE;
            if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
                mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
            ImGui::SameLine();
            if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
                mCurrentGizmoOperation = ImGuizmo::ROTATE;
            ImGui::SameLine();
            if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
                mCurrentGizmoOperation = ImGuizmo::SCALE;

            ////entity
            glm::mat4 target = glm::mat4(1.0f);
            target = glm::translate(target, currentEntity->getComponent<Transform>().position);
            target = glm::rotate(target, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            target = glm::rotate(target, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            target = glm::rotate(target, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            target = glm::scale(target, currentEntity->getComponent<Transform>().scale);
            
            static bool useSnap(false);
            if (ImGui::IsKeyPressed(83)){
                useSnap = !useSnap;
            }
            glm::vec2 snap = glm::vec2(0.25f);

            ImGuizmo::Manipulate(
                glm::value_ptr(view), 
                glm::value_ptr(projection), 
                mCurrentGizmoOperation, 
                mCurrentGizmoMode, 
                glm::value_ptr(target),
                0, 
                useSnap ? &snap.x : NULL
            );

            if (ImGuizmo::IsUsing()){
                glm::vec3 skew;
                glm::vec4 perspective;

                glm::quat orientation;;

                glm::decompose(
                    target, 
                    currentEntity->getComponent<Transform>().scale,
                    orientation,
                    currentEntity->getComponent<Transform>().position, 
                    skew, 
                    perspective);

                //orientation = glm::conjugate(orientation);

                currentEntity->getComponent<Transform>().rotation -= glm::degrees(glm::eulerAngles(orientation)); //TODO fix me still have not found out what is wrong with rotation

            }
            

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