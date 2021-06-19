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
        
        entity = new Entity("UV_Cube_0");
        GetManager()->addEntity(entity);

        entity->addComponent<Mesh>("cube.obj");
        entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Base_Color.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Normal.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Metallic.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Roughness.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_AO.png");
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
        entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Base_Color.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Normal.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Metallic.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Roughness.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_AO.png");
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
        entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Base_Color.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Normal.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Metallic.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Roughness.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_AO.png");
        entity->getComponent<ShaderProgram>().LoadTextures();
        entity->getComponent<Transform>().position = glm::vec3(4.337f, 1, -1.3525f);
        entity->getComponent<Transform>().rotation = glm::vec3(0, 65.5, 0);
        entity->getComponent<Transform>().scale = glm::vec3(2);
        entity->addComponent<script_simplebehaviours>();
        entity->getComponent<script_simplebehaviours>().SetRotateActive(true);
        entity->getComponent<script_simplebehaviours>().SetRotationSpeed(70);

        entity = new Entity("StanfordDragon");
        GetManager()->addEntity(entity);

        entity->addComponent<Mesh>("TheStanfordDragon_83k.obj");
        entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

        entity->getComponent<ShaderProgram>().AddTexturePath("");
        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Normal.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Metallic.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Roughness.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_AO.png");
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
        entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

        entity->getComponent<ShaderProgram>().AddTexturePath("Ground/WhiteTiles02_4K_BaseColor.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("Ground/WhiteTiles02_4K_Normal.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("Ground/WhiteTiles02_4K_Height.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("Ground/WhiteTiles02_4K_Roughness_e.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("Ground/WhiteTiles02_4K_AO.png");
        entity->getComponent<ShaderProgram>().LoadTextures();
        entity->getComponent<ShaderProgram>().SetTextureScale(glm::vec2(4, 4));
        entity->getComponent<Transform>().position = glm::vec3(0, -2, 0);
        entity->getComponent<Transform>().scale = glm::vec3(10, 1, 10);


        entity = new Entity("Directional Light");
        GetManager()->addEntity(entity);
        entity->getComponent<Transform>().position = glm::vec3(4.653f,5.84f,-5.234f);
        entity->addComponent<LightObject>();
        entity->addComponent<script_simplebehaviours>();


        std::vector<Entity*> lights = manager->FindLights();

        DepthBuffer d(1024 * 4, 1024 * 4);
        DepthBuffer s(1024 * 4, 1024 * 4);
        depthBuffers.emplace_back(d);
        depthBuffers.emplace_back(s);
        
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
        depthTexturesThisFrame.clear();
        for (auto& buffer : depthBuffers) {
            buffer.Bind();
            GetManager()->OnRender();
            SetShadowMap(buffer.depthMap); // this needs to become list/array
            depthTexturesThisFrame.emplace_back(buffer.depthMap);
            buffer.UnBind();
        }

        std::cout << "Amount Of Depth Textures : " << depthTexturesThisFrame.size() << std::endl;

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

        int counter = 0;
        for (auto& buffer : depthTexturesThisFrame) {
            std::string name = "DEPTH TEXTURE ";
            name.append(std::to_string(counter));
            if (ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
                int adjustedWidth = ImGui::GetContentRegionAvailWidth() * 1024 / 1024;
                ImGui::Image((void*)GetShadowMap(), ImVec2(ImGui::GetContentRegionAvailWidth(), adjustedWidth));
            }
            counter++;
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

