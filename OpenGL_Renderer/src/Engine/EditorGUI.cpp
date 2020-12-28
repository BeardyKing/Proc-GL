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


            ////entity
            glm::mat4 target = currentEntity->getComponent<Transform>().GetTransform();

            ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, glm::value_ptr(target));

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

                //currentEntity->getComponent<Transform>().rotation = glm::degrees(glm::eulerAngles(orientation)); //TODO fix me

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

bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale)
{
	// From glm::decompose in matrix_decompose.inl

	using namespace glm;
	using T = float;

	mat4 LocalMatrix(transform);

	// Normalize the matrix.
	if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
		return false;

	// First, isolate perspective.  This is the messiest.
	if (
		epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
		epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
		epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
	{
		// Clear the perspective partition
		LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
		LocalMatrix[3][3] = static_cast<T>(1);
	}

	// Next take care of translation (easy).
	translation = vec3(LocalMatrix[3]);
	LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

	vec3 Row[3], Pdum3;

	// Now get scale and shear.
	for (length_t i = 0; i < 3; ++i)
		for (length_t j = 0; j < 3; ++j)
			Row[i][j] = LocalMatrix[i][j];

	// Compute X scale factor and normalize first row.
	/*scale.x = length(Row[0]);
	Row[0] = detail::scale(Row[0], static_cast<T>(1));
	scale.y = length(Row[1]);
	Row[1] = detail::scale(Row[1], static_cast<T>(1));
	scale.z = length(Row[2]);
	Row[2] = detail::scale(Row[2], static_cast<T>(1));*/

	// At this point, the matrix (in rows[]) is orthonormal.
	// Check for a coordinate system flip.  If the determinant
	// is -1, then negate the matrix and the scaling factors.
#if 0
	Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
	if (dot(Row[0], Pdum3) < 0)
	{
		for (length_t i = 0; i < 3; i++)
		{
			scale[i] *= static_cast<T>(-1);
			Row[i] *= static_cast<T>(-1);
		}
	}
#endif

	rotation.y = asin(-Row[0][2]);
	if (cos(rotation.y) != 0) {
		rotation.x = atan2(Row[1][2], Row[2][2]);
		rotation.z = atan2(Row[0][1], Row[0][0]);
	}
	else {
		rotation.x = atan2(-Row[2][0], Row[1][1]);
		rotation.z = 0;
	}


	return true;
}