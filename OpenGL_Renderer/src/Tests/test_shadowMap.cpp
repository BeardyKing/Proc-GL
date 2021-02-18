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

bool renderShadowMap = false;
bool RenderShadowMap() {
    return renderShadowMap;
}
void SetRenderShadowMap(bool b) {
    renderShadowMap = b;
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

        entity = new Entity("Friendly default pipe_0");
        GetManager()->addEntity(entity);

        entity->getComponent<Transform>().position = glm::vec3(0, 0, 0);
        entity->addComponent<ShaderProgram>("Shaders/Blinn-Phong/Blinn-Phong.vert", "Shaders/Blinn-Phong/Blinn-Phong.frag", "Uniform_Blinn-Phong");       
        entity->addComponent<Mesh>("mesh/pipe.obj");
        entity->getComponent<ShaderProgram>().AddTexturePath("mesh/polygon_texture.png");
        entity->getComponent<ShaderProgram>().LoadTextures();

        entity = new Entity("Friendly default pipe_1");
        GetManager()->addEntity(entity);

        entity->getComponent<Transform>().position = glm::vec3(0, 0, 0);
        entity->addComponent<ShaderProgram>("Shaders/Blinn-Phong/Blinn-Phong.vert", "Shaders/Blinn-Phong/Blinn-Phong.frag", "Uniform_Blinn-Phong");
        entity->addComponent<Mesh>("mesh/pipe.obj");
        entity->getComponent<ShaderProgram>().AddTexturePath("mesh/polygon_texture.png");
        entity->getComponent<ShaderProgram>().LoadTextures();

        entity = new Entity("UV_Cube_0");
        GetManager()->addEntity(entity);

        entity->addComponent<Mesh>("cube.obj");
        entity->addComponent<ShaderProgram>("Shaders/Blinn-Phong/Blinn-Phong.vert", "Shaders/Blinn-Phong/Blinn-Phong.frag", "Uniform_Blinn-Phong");

        entity->getComponent<ShaderProgram>().AddTexturePath("UV.png");
        entity->getComponent<ShaderProgram>().LoadTextures();
        entity->getComponent<Transform>().position = glm::vec3(-3, 0, 0);

        entity = new Entity("UV_Cube_1");
        GetManager()->addEntity(entity);

        entity->addComponent<Mesh>("cube.obj");
        entity->addComponent<ShaderProgram>("Shaders/Blinn-Phong/Blinn-Phong.vert", "Shaders/Blinn-Phong/Blinn-Phong.frag", "Uniform_Blinn-Phong");

        entity->getComponent<ShaderProgram>().AddTexturePath("UV.png");
        entity->getComponent<ShaderProgram>().LoadTextures();
        entity->getComponent<Transform>().position = glm::vec3(-3, 0, 0);

        entity = new Entity("Ground");
        GetManager()->addEntity(entity);

        entity->addComponent<Mesh>("cube.obj");
        entity->addComponent<ShaderProgram>("Shaders/Blinn-Phong/Blinn-Phong.vert", "Shaders/Blinn-Phong/Blinn-Phong.frag", "Uniform_Blinn-Phong");

        entity->getComponent<ShaderProgram>().AddTexturePath("Ground/Brushed_Metal_Tiles_04_1K_AO.png");
        entity->getComponent<ShaderProgram>().LoadTextures();

        entity->getComponent<Transform>().position = glm::vec3(0, -2, 0);
        entity->getComponent<Transform>().scale = glm::vec3(10, 1, 10);


        entity = new Entity("Directional Light");
        GetManager()->addEntity(entity);
        entity->getComponent<Transform>().position = glm::vec3(8,12,2);
        entity->getComponent<Transform>().scale = glm::vec3(0.3f);
        entity->getComponent<Transform>().rotation = glm::vec3(0,-90,-90);
        entity->addComponent<LightObject>();

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
        static const unsigned int SHADOW_WIDTH = 1024*4, SHADOW_HEIGHT = 1024 * 4;
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
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        SetRenderShadowMap(true);

        // 1. render depth of scene to texture (from light's perspective)
        // --------------------------------------------------------------
        //glm::mat4 lightProjection, lightView;
        //glm::mat4 lightSpaceMatrix;
        //float near_plane = 1.0f, far_plane = 7.5f;
        ////lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
        //lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        //lightView = glm::lookAt(GetManager()->FindLights()[0]->getComponent<Transform>().position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        //lightSpaceMatrix = lightProjection * lightView;
        //// render scene from light's point of view
        //depthShader->use();
        //depthShader->setUniform("lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0, 1024*4, 1024 * 4);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glDisable(GL_CULL_FACE);
        GetManager()->OnRender();
        glEnable(GL_CULL_FACE);        
        //renderScene(simpleDepthShader);
        SetShadowMap(depthMap);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // reset viewport
        glViewport(0, 0, cam.ImGuiWindowSize.x, cam.ImGuiWindowSize.y);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        SetRenderShadowMap(false);

        
        fbo.Bind();

       

        

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GetManager()->OnRender();
        
        fbo.UnBind();
    }

    void test_shadowMap::OnExit() {
        GetManager()->OnExit();
    }

    void test_shadowMap::OnImGuiRender() {
       
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        ImGui::Begin("Debug_FrameBuffers");
        if (ImGui::CollapsingHeader("FBO_DepthBuffer", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
            int adjustedWidth = ImGui::GetContentRegionAvailWidth() * 1024 / 1024;
            ImGui::Image((void*)GetShadowMap(), ImVec2(ImGui::GetContentRegionAvailWidth(), adjustedWidth));
        }
        if (ImGui::CollapsingHeader("FBO_For_ImGui", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
            int adjustedWidth = ImGui::GetContentRegionAvailWidth() * 1024 / 1024;
            ImGui::Image((void*)fbo.renderedTexture, ImVec2(ImGui::GetContentRegionAvailWidth(), adjustedWidth));
        }
        ImGui::End();

        editor->RenderScene(fbo.renderedTexture);
        editor->RenderHierarchy();
        editor->RenderProject();
        editor->RenderConsole();
        editor->RenderMainMenuBar();
        editor->RenderActiveInspector();
    }
}

