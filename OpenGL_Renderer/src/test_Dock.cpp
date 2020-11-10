#include "test_Dock.h"

#include <iostream>
#include <sstream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

    //TODO UPDATE THIS TO TAKE IN WINDOW SIZE CHANGES
namespace test {
	test_Dock::test_Dock():
        m_fpsCamera(glm::vec3(0.0f, 0.0f, 18.0f))
    {
        m_Texture2D.loadTexture("meat.png", false);

        /// <summary>
        /// update these values on size change of window / imgui window size
        /// </summary>
        FramebufferName = 0;
        glGenFramebuffers(1, &FramebufferName);
        glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
        glGenTextures(1, &renderedTexture);
        glBindTexture(GL_TEXTURE_2D, renderedTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0); 

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
        GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    }

	test_Dock::~test_Dock() {}

	void test_Dock::OnUpdate(double deltaTime) {}

	void test_Dock::OnRender() {

        
        // Always check that our framebuffer is ok
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
            std::cout << "ERR"<<std::endl;
            return;
        }

            // Render to our framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //----------------------------------//
        //				MVP					//
        //----------------------------------//
        glm::mat4 model, view, projection;
        view = m_fpsCamera.GetViewMatrix();
        projection = glm::perspective(m_fpsCamera.getFOV(), (float)1024 / (float)768, 0.1f, 100.0f);

        //----------------------------------//
        glm::vec3 viewPos = m_fpsCamera.GetPosition();
        //----------------------------------//

        model = glm::mat4(1.0f);
        model =
            glm::translate(model, m_LightObject.position) *
            glm::rotate(model, glm::radians(m_LightObject.rotation.x), glm::vec3(1, 0, 0)) *
            glm::rotate(model, glm::radians(m_LightObject.rotation.y), glm::vec3(0, 1, 0)) *
            glm::rotate(model, glm::radians(m_LightObject.rotation.z), glm::vec3(0, 0, 1)) *
            glm::scale(model, m_LightObject.scale);

        //----------------------------------//
        //				MVP					//
        //----------------------------------//

        m_LightObject.m_Shader->use();
        m_LightObject.m_Shader->setUniform("lightCol", m_LightObject.color);
        m_LightObject.m_Shader->setUniform("model", model);
        m_LightObject.m_Shader->setUniform("view", view);
        m_LightObject.m_Shader->setUniform("projection", projection);




        m_LightObject.m_Mesh->Draw();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        m_LightObject.m_Mesh->Draw();

    }

    void test_Dock::OnImGuiRender() {
        {
            ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
            ImGui::Begin("Scene");

            ImTextureID tex = (void*)renderedTexture;
            //ImTextureID tex = (void*)m_Texture2D.GetTexture();

            float my_tex_w = (float)1024/4;
            float my_tex_h = (float)768/4;
            {
                lastFrameWindowSize = ImGui::GetWindowSize();
                ImGui::Text("%.0fx%.0f", my_tex_w, my_tex_h);
                ImGui::Text("Window Size X");
                ImGui::Text("%.0fx%.0f", lastFrameWindowSize.x, lastFrameWindowSize.y);
                ImVec2 pos = ImGui::GetCursorScreenPos();
                ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
                ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
                ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
                ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
                ImGui::Image(tex, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);
                ImGui::End();
            }
            
            {
                ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
                ImGui::Begin("Inspector");
                {


                    char label[128] = "GameObject Label";
                    char buf[128] = "";
                    //sprintf_s(buf, "GameObject %d", selectedGameObjectID);

                    ImGui::InputText(label, buf, (int)(sizeof(buf) / sizeof(*(buf))));


                    ImGui::Separator();
                    ImGui::Text("Transform");

                    ImGui::Text("Position : "); ImGui::SameLine();
                    ImGui::DragFloat3("P", &m_LightObject.position.x, -0.1f, 0.1f);

                    ImGui::Text("Rotation : "); ImGui::SameLine();
                    ImGui::DragFloat3("R", &m_LightObject.rotation.x, -1.0f, 1.0f);

                    ImGui::Text("Scale :    "); ImGui::SameLine();
                    ImGui::DragFloat3("S", &m_LightObject.scale.x, -0.1f, 0.1f);


                    ImGui::Separator();

                }
                ImGui::End();
            }
            if (true)
            {
                return;
            }

            bool show_demo_window = true;
            bool show_another_window = false;
            ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
            ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
            static int selectedGameObjectID = 0;
            static std::string selectedGameObjectName = "";



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
            if (show_demo_window)
                ImGui::ShowDemoWindow(&show_demo_window);
        }
    }
}

