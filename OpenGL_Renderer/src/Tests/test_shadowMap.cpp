#include "test_shadowMap.h"
#include "../Engine/Engine_UtilityFunctions.h"
#include "../Components/Terrain.h"

GLuint ShadowMap;
GLuint G_GetShadowMap() {return ShadowMap;}
void G_SetShadowMap(GLuint tex) {ShadowMap = tex;}
bool renderShadowMap = false;
bool G_RenderShadowMap() { return renderShadowMap;}
void G_SetRenderShadowMap( bool b) {renderShadowMap = b;}
//TODO -- Move this whole global thing to a class -- //TODO

namespace test {
    test_shadowMap::test_shadowMap(){
    
    #pragma region ECS

        editor = new EditorGUI;
        EntityManager* manager = new EntityManager;
        G_SetManager(manager);

    #pragma endregion

    #pragma region Camera

        entity = new Entity("Main Camera");
        entity->addComponent<FPSCamera>(glm::vec3(0), 4.781f, -0.549f);
        auto& cam = entity->getComponent<FPSCamera>().usingImGuiWindow = true;
        entity->getComponent<Transform>().position = glm::vec3(17, 11, -.01f);
        G_GetManager()->addEntity(entity);

    #pragma endregion
        
    #pragma region Cubemap

        std::vector<std::string> faces = {
            "skybox/right.jpg",
            "skybox/left.jpg",
            "skybox/top.jpg",
            "skybox/bottom.jpg",
            "skybox/front.jpg",
            "skybox/back.jpg"
        };

        e_skybox = new Entity("Skybox");
        G_GetManager()->addEntity(e_skybox);

        e_skybox->addComponent<Mesh>("cube.obj");
        e_skybox->addComponent<Texture2D>();
        e_skybox->getComponent<Texture2D>().LoadCubemap(faces);
        e_skybox->addComponent<ShaderProgram>("Shaders/skybox/skybox.vert", "Shaders/skybox/skybox.frag","Uniform_Skybox");
        e_skybox->getComponent<ShaderProgram>().AddTexturePath(faces[0]);
        e_skybox->getComponent<ShaderProgram>().AddTexturePath(faces[1]);
        e_skybox->getComponent<ShaderProgram>().AddTexturePath(faces[2]);
        e_skybox->getComponent<ShaderProgram>().AddTexturePath(faces[3]);
        e_skybox->getComponent<ShaderProgram>().AddTexturePath(faces[4]);
        e_skybox->getComponent<ShaderProgram>().AddTexturePath(faces[5]);
        e_skybox->getComponent<ShaderProgram>().LoadTextures(); 
        
    #pragma endregion

    #pragma region Terrain

        entity = new Entity("Terrain");
        G_GetManager()->addEntity(entity);

        entity->addComponent<Terrain>();
        entity->addComponent<Mesh>();
        entity->getComponent<Mesh>().LoadTerrain(entity->getComponent<Terrain>().GetVertexData());
        
        entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");
        entity->getComponent<ShaderProgram>().AddTexturePath("Ground/WhiteTiles02_1K_BaseColor.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("Ground/WhiteTiles02_1K_Normal.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("Ground/WhiteTiles02_1K_Height.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("Ground/WhiteTiles02_1K_Roughness_e.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("Ground/WhiteTiles02_1K_AO.png");
        entity->getComponent<ShaderProgram>().LoadTextures();
        entity->getComponent<ShaderProgram>().SetFloat(0.723f, "normal_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(0.207f, "metallic_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(0.199f, "roughness_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(9.817f, "occlusion_scalar");
        entity->getComponent<ShaderProgram>().SetTextureScale(glm::vec2(4, 4));

        entity->getComponent<Transform>().scale = glm::vec3(0.01f);
        entity->getComponent<Transform>().position = glm::vec3(-80,-25,-60);

    #pragma endregion

    #pragma region Marble_Ground
        entity = new Entity("marble ground");
        G_GetManager()->addEntity(entity);

        entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");
        
        entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

        entity->addComponent<Mesh>("8502_Assets/marble_ground/Marble_Ground.obj");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_diff_1k.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_nor_1k.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_rough_1k.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_AO_1k.png");
        entity->getComponent<ShaderProgram>().LoadTextures();
        entity->getComponent<ShaderProgram>().SetFloat(1.0f, "normal_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(1.0f, "metallic_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(1.0f, "roughness_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(1.0f, "occlusion_scalar");
    #pragma endregion


    #pragma region Statue



        entity = new Entity("Afrodta Statue");
        G_GetManager()->addEntity(entity);

        // 0 = albedo
        // 1 = normal
        // 2 = metalic
        // 3 = roughness
        // 4 = ambient 

        entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");
        
        entity->addComponent<Mesh>("Afrodta_Statue/Rz_123_Afrodyta_z_Melos.obj");
        entity->getComponent<ShaderProgram>().AddTexturePath("Afrodta_Statue/Rz_123_Afrodyta_z_Melos_albedo.jpg");
        entity->getComponent<ShaderProgram>().AddTexturePath("Afrodta_Statue/Rz_123_Afrodyta_z_Melos_normal.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Metallic.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Roughness.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_AO.png");
        entity->getComponent<ShaderProgram>().LoadTextures();
        
        entity->getComponent<ShaderProgram>().SetFloat(0.864f, "normal_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(0.087f, "metallic_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(0.322f, "roughness_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(4.6f, "occlusion_scalar");

    #pragma endregion

    #pragma region Directional_Light

        entity = new Entity("Directional Light");
        G_GetManager()->addEntity(entity);
        entity->getComponent<Transform>().position = glm::vec3(0.8f,15.0f,-7.2f);
        entity->addComponent<LightObject>();
        entity->addComponent<script_simplebehaviours>();

        std::vector<Entity*> lights = manager->FindLights();

    #pragma endregion

    #pragma region Frame Buffer (imGUI Image)

        DepthBuffer d(1024 * 4, 1024 * 4);
        DepthBuffer s(1024 * 4, 1024 * 4);
        depthBuffers.emplace_back(d);
        depthBuffers.emplace_back(s);
        
        fbo.GenerateFrameBuffer(editor->lastFrameWindowSize.x, editor->lastFrameWindowSize.y);

    #pragma endregion

    }

    test_shadowMap::~test_shadowMap() {
        delete entity;
        delete editor;
        delete camera;
        delete depthShader;
        delete e_skybox;

        fbo.UnBind();
        glViewport(0, 0, G_GetWindowWidth(), G_GetWindowHeight());
    }

	void test_shadowMap::OnUpdate(double deltaTime) {
        G_GetManager()->OnUpdate(deltaTime);
    }
    bool once = false;
	void test_shadowMap::OnRender() {
        if (editor->windowSizeChangeFlag) {
            editor->UpdateFrameBufferTextureSize(fbo.GetRenderBuffer());
        }

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            return;
        }

        depthTexturesThisFrame.clear();
        for (auto& buffer : depthBuffers) {
            buffer.Bind();
            G_GetManager()->OnRender();
            G_SetShadowMap(buffer.GetDepthBuffer()); // this needs to become list/array
            depthTexturesThisFrame.emplace_back(buffer.GetDepthBuffer());
            buffer.UnBind();
        }

        fbo.Bind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        G_GetManager()->OnRender();
        
        fbo.UnBind();
    }

    void test_shadowMap::OnExit() {
        G_GetManager()->OnExit();
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
                ImGui::Image((void*)G_GetShadowMap(), ImVec2(ImGui::GetContentRegionAvailWidth(), adjustedWidth));
            }
            counter++;
        }

        if (ImGui::CollapsingHeader("FBO_For_ImGui", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
            int adjustedWidth = ImGui::GetContentRegionAvailWidth() * 1024 / 1024;
            ImGui::Image((void*)fbo.GetRenderBuffer(), ImVec2(ImGui::GetContentRegionAvailWidth(), adjustedWidth));
        }
        ImGui::End();

        editor->RenderScene(fbo.GetRenderBuffer());
        editor->RenderHierarchy();
        editor->RenderProject();
        editor->RenderConsole();
        editor->RenderMainMenuBar();
        editor->RenderActiveInspector();
    }
}

