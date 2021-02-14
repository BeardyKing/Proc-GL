#include "test_shadowMap.h"

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
GLuint ShadowMap;
GLuint GetShadowMap() {
    return ShadowMap;
}
void SetShadowMap(GLuint tex) {
     ShadowMap = tex;
}
namespace test {
    test_shadowMap::test_shadowMap(){
        editor = new EditorGUI;
        EntityManager* manager = new EntityManager;
        SetManager(manager);

        entity = new Entity("Main Camera");
        entity->addComponent<FPSCamera>();
        auto& cam = entity->getComponent<FPSCamera>().usingImGuiWindow = true;
        entity->getComponent<Transform>().position = glm::vec3(0, 0, -18);
        GetManager()->addEntity(entity);

        entity = new Entity("Friendly default pipe");
        GetManager()->addEntity(entity);

        entity->getComponent<Transform>().position = glm::vec3(0, 0, 0);
        entity->addComponent<ShaderProgram>("Shaders/Blinn-Phong/Blinn-Phong.vert", "Shaders/Blinn-Phong/Blinn-Phong.frag", "Uniform_Blinn-Phong");       
        entity->addComponent<Mesh>("mesh/pipe.obj");
        entity->getComponent<ShaderProgram>().AddTexturePath("mesh/polygon_texture.png");
        entity->getComponent<ShaderProgram>().LoadTextures();



        entity = new Entity("Ground");
        GetManager()->addEntity(entity);

        entity->addComponent<Mesh>("cube.obj");
        entity->addComponent<ShaderProgram>("Shaders/Blinn-Phong/Blinn-Phong.vert", "Shaders/Blinn-Phong/Blinn-Phong.frag", "Uniform_Blinn-Phong");

        entity->getComponent<ShaderProgram>().AddTexturePath("Ground/Brushed_Metal_Tiles_04_1K_AO.png");
        entity->getComponent<ShaderProgram>().LoadTextures();

        entity->getComponent<Transform>().position = glm::vec3(0, -2, 0);
        entity->getComponent<Transform>().scale = glm::vec3(10, 1, 10);

        entity = new Entity("deothObj");
        GetManager()->addEntity(entity);

        entity->addComponent<Mesh>("cube.obj");
        entity->addComponent<ShaderProgram>("Shaders/ShadowMapping/shadow_depth.vert", "Shaders/ShadowMapping/shadow_depth.frag");
        entity->getComponent<Transform>().position = glm::vec3(1, 0, 0);
        debugShader = &entity->getComponent<ShaderProgram>();


        entity = new Entity("Directional Light");
        GetManager()->addEntity(entity);
        entity->getComponent<Transform>().position = glm::vec3(2);
        entity->getComponent<Transform>().scale = glm::vec3(0.3f);
        entity->getComponent<Transform>().rotation = glm::vec3(0,-90,-90);
        entity->addComponent<LightObject>();
        /*entity->addComponent<Mesh>();
        entity->addComponent<ShaderProgram>();*/


        fbo.GenerateFrameBuffer(editor->lastFrameWindowSize.x, editor->lastFrameWindowSize.y);
    }

    test_shadowMap::~test_shadowMap() {
        fbo.UnBind();
        glViewport(0, 0, G_GetWindowWidth(), G_GetWindowHeight());
    }

	void test_shadowMap::OnUpdate(double deltaTime) {
        GetManager()->OnUpdate(deltaTime);
    }
    bool once = false;
	void test_shadowMap::OnRender() {
        if (editor->windowSizeChangeFlag) {
            editor->UpdateFrameBufferTextureSize(fbo.renderedTexture);
        }

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            return;
        }
        static const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
        static unsigned int depthMapFBO;
        static unsigned int depthMap;
        if (once == false) {
            once = true;
            glGenFramebuffers(1, &depthMapFBO);
            // create depth texture
            glGenTextures(1, &depthMap);
            glBindTexture(GL_TEXTURE_2D, depthMap);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
            // attach depth texture as FBO's depth buffer
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 1. render depth of scene to texture (from light's perspective)
        // --------------------------------------------------------------
        
        auto& cam = GetManager()->FindActiveCamera()->getComponent<FPSCamera>();
        

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, woodTexture);
        glDisable(GL_CULL_FACE);
        GetManager()->OnRender();
        glEnable(GL_CULL_FACE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        SetShadowMap(depthMap);
        glViewport(0, 0, cam.ImGuiWindowSize.x, cam.ImGuiWindowSize.y);

        
        fbo.Bind();

        float near_plane = 1.0f, far_plane = 7.5f;
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

        glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, GetShadowMap());

        debugShader->setUniform("near_plane", 0.1f);
        debugShader->setUniform("far_plane", 100.0f);

        debugShader->setUniform("lightSpaceMatrix", lightSpaceMatrix);
        debugShader->setUniform("model", GetManager()->FindLights()[0]->getComponent<Transform>().GetTransformMatrix());
        debugShader->setUniformSampler("depthMap",0);

        if (ImGui::CollapsingHeader("FBO_For_ShadowMap", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
            int adjustedWidth = ImGui::GetContentRegionAvailWidth() * 1024 / 1024;
            ImGui::Image((void*)GetShadowMap(), ImVec2(ImGui::GetContentRegionAvailWidth(), adjustedWidth));
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GetManager()->OnRender();
        if (ImGui::CollapsingHeader("FBO_For_ImGui", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
            int adjustedWidth = ImGui::GetContentRegionAvailWidth() * 1024 / 1024;
            ImGui::Image((void*)fbo.renderedTexture, ImVec2(ImGui::GetContentRegionAvailWidth(), adjustedWidth));
        }
        fbo.UnBind();
    }

    void test_shadowMap::OnExit() {
        GetManager()->OnExit();
    }

    void test_shadowMap::OnImGuiRender() {
    
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        editor->RenderScene(fbo.renderedTexture);
        editor->RenderHierarchy();
        editor->RenderProject();
        editor->RenderConsole();
        editor->RenderMainMenuBar();
        editor->RenderActiveInspector();
    }
}

