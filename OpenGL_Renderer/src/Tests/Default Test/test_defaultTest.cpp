#include "test_defaultTest.h"

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
    test_Default::test_Default(){
        editor = new EditorGUI;
        EntityManager* manager = new EntityManager;
        SetManager(manager);

        entity = new Entity("Main Camera");
        entity->addComponent<FPSCamera>();
        auto& cam = entity->getComponent<FPSCamera>().usingImGuiWindow = true;
        entity->getComponent<Transform>().position = glm::vec3(0, 0, -18);
        GetManager()->addEntity(entity);

        entity = new Entity("Friendly default pipe");
        entity->getComponent<Transform>().position = glm::vec3(0, 0, 0);
        entity->addComponent<Mesh>("mesh/pipe.obj");
        entity->addComponent<ShaderProgram>();
        GetManager()->addEntity(entity);

        fbo.GenerateFrameBuffer(editor->lastFrameWindowSize.x, editor->lastFrameWindowSize.y);
    }

    test_Default::~test_Default() {
        fbo.UnBind();
        glViewport(0, 0, G_GetWindowWidth(), G_GetWindowHeight());
    }

	void test_Default::OnUpdate(double deltaTime) {
        GetManager()->OnUpdate(deltaTime);
    }

	void test_Default::OnRender() {
        if (editor->windowSizeChangeFlag){
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

    void test_Default::OnExit() {
        GetManager()->OnExit();
    }

    void test_Default::OnImGuiRender() {
    
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        editor->RenderScene(fbo.renderedTexture);
        editor->RenderHierarchy();
        editor->RenderProject();
        editor->RenderConsole();
        editor->RenderMainMenuBar();
        editor->RenderActiveInspector();
    }
}

