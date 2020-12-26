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
	test_Dock::test_Dock(){
        editor = new EditorGUI;
        EntityManager* manager = new EntityManager;
        SetManager(manager);

        entity = new Entity("Main Camera");
        entity->addComponent<FPSCamera>();
        auto& cam = entity->getComponent<FPSCamera>().usingImGuiWindow = true;
        entity->getComponent<Transform>().position = glm::vec3(0, 0, -18);
        GetManager()->addEntity(entity);


        for (size_t i = 0; i < 100; i++){
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

        fbo.GenerateFrameBuffer(editor->lastFrameWindowSize.x, editor->lastFrameWindowSize.y);
    }

	test_Dock::~test_Dock() {
        fbo.UnBind();
        glViewport(0, 0, G_GetWindowWidth(), G_GetWindowHeight());
    }

	void test_Dock::OnUpdate(double deltaTime) {
        GetManager()->OnUpdate(deltaTime);
    }

	void test_Dock::OnRender() {
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

    void test_Dock::OnExit() {
        GetManager()->OnExit();
    }

    void test_Dock::OnImGuiRender() {
    
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        editor->RenderScene(fbo.renderedTexture);
        editor->RenderHierarchy();
        editor->RenderProject();
        editor->RenderConsole();
        editor->RenderMainMenuBar();
        editor->RenderActiveInspector();
        
    }
}

