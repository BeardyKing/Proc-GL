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
        entity->addComponent<FPSCamera>(glm::vec3(0), 4.781f, -0.549f);
        auto& cam = entity->getComponent<FPSCamera>().usingImGuiWindow = true;
        entity->getComponent<Transform>().position = glm::vec3(17, 11, -.01f);
        GetManager()->addEntity(entity);

        entity = new Entity("Friendly default pipe_0");
        GetManager()->addEntity(entity);

        entity->getComponent<Transform>().position = glm::vec3(1.736f, 3.163, 4.234);
        entity->getComponent<Transform>().rotation = glm::vec3(0, 66.5, 0);
        entity->addComponent<ShaderProgram>("Shaders/Blinn-Phong/Blinn-Phong.vert", "Shaders/Blinn-Phong/Blinn-Phong.frag", "Uniform_Blinn-Phong");       
        entity->addComponent<Mesh>("mesh/pipe.obj");
        entity->getComponent<ShaderProgram>().AddTexturePath("mesh/polygon_texture.png");
        entity->getComponent<ShaderProgram>().LoadTextures();
        entity->addComponent<script_simplebehaviours>();
        entity->getComponent<script_simplebehaviours>().SetRotateActive(true);

        
        entity = new Entity("UV_Cube_0");
        GetManager()->addEntity(entity);

        entity->addComponent<Mesh>("cube.obj");
        entity->addComponent<ShaderProgram>("Shaders/Blinn-Phong/Blinn-Phong.vert", "Shaders/Blinn-Phong/Blinn-Phong.frag", "Uniform_Blinn-Phong");

        entity->getComponent<ShaderProgram>().AddTexturePath("UV.png");
        entity->getComponent<ShaderProgram>().LoadTextures();
        entity->getComponent<Transform>().position = glm::vec3(1, 0, 6.5);
        entity->getComponent<Transform>().rotation = glm::vec3(0, 6, 0);
        entity->getComponent<Transform>().scale = glm::vec3(0.67f);

        entity->addComponent<script_simplebehaviours>();
        entity->getComponent<script_simplebehaviours>().SetRotateActive(true);
        entity->getComponent<script_simplebehaviours>().SetRotationSpeed(-45);

        entity = new Entity("UV_Cube_1");
        GetManager()->addEntity(entity);

        entity->addComponent<Mesh>("cube.obj");
        entity->addComponent<ShaderProgram>("Shaders/Blinn-Phong/Blinn-Phong.vert", "Shaders/Blinn-Phong/Blinn-Phong.frag", "Uniform_Blinn-Phong");

        entity->getComponent<ShaderProgram>().AddTexturePath("UV.png");
        entity->getComponent<ShaderProgram>().LoadTextures();
        entity->getComponent<Transform>().position = glm::vec3(1.183f, 1.1f, 3.525f);
        entity->getComponent<Transform>().rotation = glm::vec3(0, 17.5, 0);
        entity->getComponent<Transform>().scale = glm::vec3(1, 2, 2);
        entity->addComponent<script_simplebehaviours>();
        entity->getComponent<script_simplebehaviours>().SetRotateActive(true);
        entity->getComponent<script_simplebehaviours>().SetRotationSpeed(70);

        entity = new Entity("UV_Cube_2");
        GetManager()->addEntity(entity);

        entity->addComponent<Mesh>("cube.obj");
        entity->addComponent<ShaderProgram>("Shaders/Blinn-Phong/Blinn-Phong.vert", "Shaders/Blinn-Phong/Blinn-Phong.frag", "Uniform_Blinn-Phong");

        entity->getComponent<ShaderProgram>().AddTexturePath("UV.png");
        entity->getComponent<ShaderProgram>().LoadTextures();
        entity->getComponent<Transform>().position = glm::vec3(4.337f, 1, -1.3525f);
        entity->getComponent<Transform>().rotation = glm::vec3(0, 65.5, 0);
        entity->getComponent<Transform>().scale = glm::vec3(2);
        entity->addComponent<script_simplebehaviours>();
        entity->getComponent<script_simplebehaviours>().SetRotateActive(true);
        entity->getComponent<script_simplebehaviours>().SetRotationSpeed(70);

        entity = new Entity("StanfordDragon");
        GetManager()->addEntity(entity);

        entity->addComponent<Mesh>("StanfordDragon.obj");
        entity->addComponent<ShaderProgram>("Shaders/Blinn-Phong/Blinn-Phong.vert", "Shaders/Blinn-Phong/Blinn-Phong.frag", "Uniform_Blinn-Phong");

        entity->getComponent<ShaderProgram>().AddTexturePath("UV.png");
        entity->getComponent<ShaderProgram>().LoadTextures();
        entity->getComponent<Transform>().position = glm::vec3(4.683f,2.865, -1.686f);
        entity->getComponent<Transform>().rotation = glm::vec3(-180, 65, -180);
        entity->getComponent<Transform>().scale = glm::vec3(24.6f);
        entity->addComponent<script_simplebehaviours>();
        
        entity = new Entity("PBR cube");
        GetManager()->addEntity(entity);

        entity->getComponent<Transform>().scale = glm::vec3(3);
        entity->addComponent<Mesh>("cube.obj");
        entity->addComponent<ShaderProgram>("pbr.vert", "pbr.frag", "Uniform_PBR");
        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Base_Color.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Normal.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Metallic.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Roughness.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_AO.png");
        entity->getComponent<ShaderProgram>().LoadTextures();
        entity->getComponent<Transform>().position = glm::vec3(5.986f, -2.940, -4.017f);
        entity->getComponent<Transform>().scale = glm::vec3(2.0f);


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
        entity->getComponent<Transform>().position = glm::vec3(4.653f,5.84f,-5.234f);
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
        static const unsigned int SHADOW_WIDTH = 1024*4, SHADOW_HEIGHT = 1024*4;
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

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glCullFace(GL_NONE);
        GetManager()->OnRender();
        glCullFace(GL_BACK);
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

