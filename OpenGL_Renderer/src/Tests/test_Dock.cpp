#include "test_Dock.h"

#include <iostream>
#include <sstream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
extern uint32_t GetAmountOfEntities();

extern void SetManager(EntityManager* mgr);
EntityManager* GetManager();

namespace test {
	test_Dock::test_Dock():
        lastFrameWindowSize(ImVec2(0, 0)),
        windowSizeChangeFlag(false)    {

        EntityManager* manager = new EntityManager;
        SetManager(manager);

        entity = new Entity("Main Camera");
        entity->addComponent<FPSCamera>();
        auto& cam = entity->getComponent<FPSCamera>().usingImGuiWindow = false;
        entity->getComponent<Transform>().position = glm::vec3(0, 0, -18);
        GetManager()->addEntity(entity);


        for (size_t i = 0; i < 100; i++)
        {
            std::string name = "basic sphere ";
            name.append(std::to_string(i));
            entity = new Entity(name.c_str());
            GetManager()->addEntity(entity);
            entity->addComponent<ShaderProgram>();
            entity->getComponent<ShaderProgram>().SetBaseColor(glm::vec3(glm::ballRand((float)i / 2)));
            if (i % 2) {
                entity->addComponent<Mesh>("objectDefaults/cube.obj");
            }
            else if (i % 3) {
                entity->addComponent<Mesh>("mesh/pipe.obj");
            }
            else {
                entity->addComponent<Mesh>();
            }
            entity->getComponent<Transform>().position = glm::vec3(glm::ballRand((float)i / 2));
            entity->getComponent<Transform>().rotation = glm::vec3(glm::ballRand((float)i / 2));
        }

        entity = new Entity();
        GetManager()->addEntity(entity);


        entity = new Entity("POINT LIGHT");
        entity->addComponent<LightObject>();
        GetManager()->addEntity(entity);

        fbo.GenerateFrameBuffer(lastFrameWindowSize.x, lastFrameWindowSize.y);
    }

	test_Dock::~test_Dock() {
        fbo.UnBind();
        glViewport(0, 0, G_GetWindowWidth(), G_GetWindowHeight());
    }

	void test_Dock::OnUpdate(double deltaTime) {
        GetManager()->OnUpdate(deltaTime);
    }

	void test_Dock::OnRender() {
        if (windowSizeChangeFlag){
            UpdateFrameBufferTextureSize();
        }
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            return;
        }

        fbo.Bind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        GetManager()->OnRender();

        fbo.UnBind();

    }

    void test_Dock::OnExit() {
        GetManager()->OnExit();
    }

    void test_Dock::OnImGuiRender() {
        {
            bool show_demo_window = false;
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
                ImVec2 uv_max = ImVec2(1.0f, -1.0f);                
                ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  
                ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
                ImGui::Image(tex, windowSize, uv_min, uv_max, tint_col, border_col);
                ImGui::End();
            }
            
            {
                GetManager()->Editor_RenderActiveEditityGui();
            }

            {
                RenderHierarchy();
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

    void test_Dock::RenderHierarchy() {
        {
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
    }

    void test_Dock::UpdateFrameBufferTextureSize() {
        glBindTexture(GL_TEXTURE_2D, fbo.renderedTexture);
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
}

