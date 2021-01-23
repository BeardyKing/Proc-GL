#include "test_imguizmo.h"

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
    test_imguizmo::test_imguizmo() {
        editor = new EditorGUI;
        EntityManager* manager = new EntityManager;
        SetManager(manager);

        entity = new Entity("Main Camera");
        entity->addComponent<FPSCamera>();
        auto& cam = entity->getComponent<FPSCamera>().usingImGuiWindow = true;
        entity->getComponent<Transform>().position = glm::vec3(0, 0, 0);
        GetManager()->addEntity(entity);


        for (int16_t i = 0; i < 3; i++) {
            std::string name = "basic sphere ";
            name.append(std::to_string(i));
            entity = new Entity(name.c_str());
            GetManager()->addEntity(entity);
            entity->addComponent<ShaderProgram>();
            entity->getComponent<ShaderProgram>().SetBaseColor(glm::vec3(3 / (i + 1), 0, 0));
            entity->addComponent<Mesh>("mesh/pipe.obj");
            entity->addComponent<script_simplebehaviours>();
            entity->getComponent<Transform>().position = glm::vec3(i*2 + -2,-2,7);
        }

        fbo.GenerateFrameBuffer(editor->lastFrameWindowSize.x, editor->lastFrameWindowSize.y);
    }

    test_imguizmo::~test_imguizmo() {
        fbo.UnBind();
        glViewport(0, 0, G_GetWindowWidth(), G_GetWindowHeight());
    }

    void test_imguizmo::OnUpdate(double deltaTime) {
        GetManager()->OnUpdate(deltaTime);
    }

    void test_imguizmo::OnRender() {
        if (editor->windowSizeChangeFlag) {
            editor->UpdateFrameBufferTextureSize(fbo.renderedTexture);
        }

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            return;
        }

        fbo.Bind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GetManager()->OnRender();

        fbo.UnBind();

    }

    void test_imguizmo::OnExit() {
        GetManager()->OnExit();
    }

    void test_imguizmo::OnImGuiRender() {

        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        editor->RenderScene(fbo.renderedTexture);
        editor->RenderHierarchy();
        editor->RenderProject();
        editor->RenderConsole();
        editor->RenderMainMenuBar();
        editor->RenderActiveInspector();






    }
}

