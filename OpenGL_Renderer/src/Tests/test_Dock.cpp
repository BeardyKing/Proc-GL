#include "test_Dock.h"

#include <iostream>
#include <sstream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>


namespace test {
	test_Dock::test_Dock():
        lastFrameWindowSize(ImVec2(0, 0)),
        windowSizeChangeFlag(false),
        m_fpsCamera(glm::vec3(0.0f, 0.0f, 18.0f))
    {
        m_Texture2D.loadTexture("meat.png", false);
        lastFrameWindowSize = ImVec2(0,0);

        fbo.GenerateFrameBuffer(lastFrameWindowSize.x, lastFrameWindowSize.y);
    }

	test_Dock::~test_Dock() {
        fbo.UnBind();
        glViewport(0, 0, G_GetWindowWidth(), G_GetWindowHeight());
    }

	void test_Dock::OnUpdate(double deltaTime) {}

	void test_Dock::OnRender() {
        if (windowSizeChangeFlag){
            UpdateFrameBufferTextureSize();
        }
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            return;
        }

        fbo.Bind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //----------------------------------//
        //				MVP					//
        //----------------------------------//
        glm::mat4 model, view, projection;
        view = m_fpsCamera.GetViewMatrix();
        projection = glm::perspective(m_fpsCamera.getFOV(), (float)lastFrameWindowSize.x / (float)lastFrameWindowSize.y, 0.1f, 100.0f);

        //----------------------------------//
        glm::vec3 viewPos = m_fpsCamera.GetPosition();
        //----------------------------------//

        model = glm::mat4(1.0f);
        model =
            glm::translate(model, m_LightObject.transform.position) *
            glm::rotate(model, glm::radians(m_LightObject.transform.rotation.x), glm::vec3(1, 0, 0)) *
            glm::rotate(model, glm::radians(m_LightObject.transform.rotation.y), glm::vec3(0, 1, 0)) *
            glm::rotate(model, glm::radians(m_LightObject.transform.rotation.z), glm::vec3(0, 0, 1)) *
            glm::scale(model, m_LightObject.transform.scale);

        //----------------------------------//
        //				MVP					//
        //----------------------------------//

        m_LightObject.m_Shader->use();
        m_LightObject.m_Shader->setUniform("lightCol", m_LightObject.color);
        m_LightObject.m_Shader->setUniform("model", model);
        m_LightObject.m_Shader->setUniform("view", view);
        m_LightObject.m_Shader->setUniform("projection", projection);

        m_LightObject.m_Mesh->Draw();

        fbo.UnBind();

    }

    void test_Dock::OnImGuiRender() {
        {
            bool show_demo_window = true;
            bool show_another_window = false;
            ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

            ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

            static int selectedGameObjectID = 0;
            static std::string selectedGameObjectName = "";

            if (show_demo_window)
                ImGui::ShowDemoWindow(&show_demo_window);
            if (G_GetWindowResizeFlag()){
                UpdateFrameBufferTextureSize();
            }

            ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
            ImGui::Begin("Scene");

            ImVec2 windowSize = ImGui::GetContentRegionAvail();

            if (windowSize.x != lastFrameWindowSize.x || windowSize.y != lastFrameWindowSize.y){
                lastFrameWindowSize = windowSize;
                windowSizeChangeFlag = true;
            }

            {
                ImTextureID tex = (void*)fbo.renderedTexture; // Texture from framebuffer

                ImVec2 pos = ImGui::GetCursorScreenPos();
                ImVec2 uv_min = ImVec2(0.0f, 0.0f);                
                ImVec2 uv_max = ImVec2(1.0f, 1.0f);                
                ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  
                ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
                ImGui::Image(tex, windowSize, uv_min, uv_max, tint_col, border_col);
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
                    ImGui::DragFloat3("P", &m_LightObject.transform.position.x, -0.1f, 0.1f);

                    ImGui::Text("Rotation : "); ImGui::SameLine();
                    ImGui::DragFloat3("R", &m_LightObject.transform.rotation.x, -1.0f, 1.0f);

                    ImGui::Text("Scale :    "); ImGui::SameLine();
                    ImGui::DragFloat3("S", &m_LightObject.transform.scale.x, -0.1f, 0.1f);


                    ImGui::Separator();

                }
                ImGui::End();
            }

            {
                ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
                ImGui::Begin("Hierarchy");
                {
                    for (int i = 0; i < 1; i++)
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
            
        }
    }

    void test_Dock::UpdateFrameBufferTextureSize() {
        glBindTexture(GL_TEXTURE_2D, fbo.renderedTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, lastFrameWindowSize.x, lastFrameWindowSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
        windowSizeChangeFlag = false;
        glViewport(0, 0, lastFrameWindowSize.x, lastFrameWindowSize.y);
    }
}

