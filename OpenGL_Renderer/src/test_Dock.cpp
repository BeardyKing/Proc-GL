#include "test_Dock.h"

#include <iostream>
#include <sstream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>



namespace test {
	test_Dock::test_Dock(){

	}

	test_Dock::~test_Dock() {}

	void test_Dock::OnUpdate(double deltaTime) {}

	void test_Dock::OnRender() {}

	void test_Dock::OnImGuiRender() {


		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        static int selectedGameObjectID = 0;
        static std::string selectedGameObjectName = "";

        {
            ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
            ImGui::Begin("Inspector");
            {
               
                
                char label[128] = "GameObject Label";
                char buf[128] = "";
                sprintf_s(buf, "GameObject %d", selectedGameObjectID);

                ImGui::InputText(label, buf, (int)(sizeof(buf) / sizeof(*(buf))));
                
                
                ImGui::Separator();
                ImGui::Text("Transform");

                ImGui::Text("Position : "); ImGui::SameLine();
                ImGui::DragFloat3("P", &m_pbrSpherePosition.x, -0.1f, 0.1f);

                ImGui::Text("Rotation : "); ImGui::SameLine();
                ImGui::DragFloat3("R", &m_pbrSphereRotationAxis.x, -1.0f, 1.0f);

                ImGui::Text("Scale :    "); ImGui::SameLine();
                ImGui::DragFloat3("S", &m_pbrSphereScale.x, -0.1f, 0.1f);


                ImGui::Separator();

            }
            ImGui::End();
        }

        {
            ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
            ImGui::Begin("Hierarchy");
            {
                for (int i = 0; i < 100; i++)
                {
                    char label[128];
                    sprintf_s(label, "GameObject %d", i);
                    if (ImGui::Selectable(label, selectedGameObjectID == i)) {
                        selectedGameObjectID = i;
                        selectedGameObjectName = (std::string)label;

                    }
                }
            }
            ImGui::SameLine();
            ImGui::End();
        }

        {
            ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
            ImGui::Begin("Project");
            ImGui::End();
        }

        {
            ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

            ImGui::Begin("Console");

            ImGui::Columns(2, "word-wrapping",true);
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

        {
            ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
            ImGui::Begin("Scene");
            ImGui::End();
        }
	


        if (ImGui::BeginMainMenuBar()){
            if (ImGui::BeginMenu("File")){
                if (ImGui::MenuItem("ITEM", "CTRL+O")) {
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")){
                if (ImGui::MenuItem("ITEM", "ACTION")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
	}
}

