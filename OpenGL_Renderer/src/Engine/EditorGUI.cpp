#include "EditorGUI.h"

#include "ECS_List.h"

#include "../Math/Engine_Math.h"



EditorGUI::EditorGUI() {
}

EditorGUI::~EditorGUI() {

}

void EditorGUI::OnGizmoRender() {

}

void EditorGUI::RenderHierarchy() {
	//std::cout << GetAmountOfEntities() << std::endl;
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
	ImGui::Begin("ECS_Hierarchy");
	{
		for (uint32_t i = 0; i < GetAmountOfEntities(); i++) {
			auto label = G_GetManager()->entities[i]->getComponent<ObjectData>().GetName();
			if (ImGui::Selectable(label, G_GetManager()->Editor_GetActiveEntity() == i)) {
				G_GetManager()->Editor_SetActiveEntity(i);
			}
		}
	}

	ImGui::SameLine();
	ImGui::End();
}

void EditorGUI::RenderScene(const GLuint& renderTexture) {
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

        //          //
        // ImGuizmo //
        //          //

        Entity* currentEntity = G_GetManager()->GetSelectedEntity();
        auto& currentEntityTransform = currentEntity->getComponent<Transform>();
        Entity* cam = NULL;

        if (!cam) {
            for (auto& m_entity : G_GetManager()->entities) {
                if (m_entity->hasComponent<FPSCamera>()) {
                    cam = m_entity.get();
                    break;
                }
            }
        }

        if (currentEntity && cam) {
            ImGui::Begin("Scene");            
            float windowWidth  = (float)ImGui::GetItemRectSize().x + 8;
            float windowHeight = (float)ImGui::GetItemRectSize().y + 8;

            static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::OPERATION::TRANSLATE);
            static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::MODE::LOCAL);
            
            if (ImGui::IsKeyPressed(49)) mCurrentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;   //key "1"
            if (ImGui::IsKeyPressed(50)) mCurrentGizmoOperation = ImGuizmo::OPERATION::ROTATE;      //key "2"
            if (ImGui::IsKeyPressed(51)) mCurrentGizmoOperation = ImGuizmo::OPERATION::SCALE;       //key "3"
            if (ImGui::IsKeyPressed(52)) mCurrentGizmoOperation = ImGuizmo::OPERATION::BOUNDS;      //key "4"
            
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			glm::mat4 camView, camProjection;
			camView = cam->getComponent<FPSCamera>().GetViewMatrix();
			camProjection = glm::perspective(glm::radians(cam->getComponent<FPSCamera>().getFOV()), windowWidth / windowHeight, 0.1f, 100.0f);
            
            glm::mat4 targetTransformMatrix = currentEntityTransform.GetTransformMatrix();
            
            static bool useSnap(false);
            glm::vec2 snap;
            if (ImGui::IsKeyPressed(83)){   useSnap = !useSnap; } //key "s"
                if (ImGuizmo::OPERATION::ROTATE)
                    snap = glm::vec2(11.25f);
                else
                    snap = glm::vec2(0.25f);

            ImGuizmo::Manipulate(
                glm::value_ptr(camView), 
                glm::value_ptr(camProjection), 
                mCurrentGizmoOperation, 
                mCurrentGizmoMode, 
                glm::value_ptr(targetTransformMatrix),
                0, 
                useSnap ? &snap.x : NULL
            );

            if (ImGuizmo::IsUsing()){
                glm::vec3 skew;
                glm::vec4 perspective;

                glm::vec3 outPosition;
                glm::quat outRotation;;
                glm::vec3 outScale;

                glm::decompose(
                    targetTransformMatrix, 
                    outScale,
                    outRotation,
                    outPosition,
                    skew, 
                    perspective); //TODO Lighter decompose function 

                outRotation = glm::conjugate(outRotation);

                glm::vec3 deltaAngle = glm::radians(currentEntityTransform.rotation) - glm::eulerAngles(outRotation);

                currentEntityTransform.scale    = outScale;
                currentEntityTransform.rotation -= glm::degrees(deltaAngle);
                currentEntityTransform.position = outPosition;
            }
            ImGui::End();
        }
        ImGui::End();
    }
}

void EditorGUI::RenderInspector() {}

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
    for (auto& entity : G_GetManager()->entities) {
        if (entity->hasComponent<FPSCamera>()) {
            auto& cam = entity->getComponent<FPSCamera>();
            cam.ImGuiWindowSize = lastFrameWindowSize;
        }
    }
}

void EditorGUI::RenderActiveInspector() {
    G_GetManager()->Editor_RenderActiveInspector();
}

void  EditorGUI::Demo(bool &demo) {
    if (&demo)
        ImGui::ShowDemoWindow(&demo);
}