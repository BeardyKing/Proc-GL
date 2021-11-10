#include "test_shadowMap.h"
#include "../Engine/Engine_UtilityFunctions.h"




GLuint ShadowMap;
GLuint G_GetShadowMap() {return ShadowMap;}
void G_SetShadowMap(GLuint tex) {ShadowMap = tex;}
bool renderShadowMap = false;
bool G_RenderShadowMap() { return renderShadowMap;}
void G_SetRenderShadowMap( bool b) {renderShadowMap = b;}
//TODO -- Move this whole global thing to a class -- //TODO


namespace test {
    test_shadowMap::test_shadowMap(){
        editor = new EditorGUI;
        EntityManager* manager = new EntityManager;
        G_SetManager(manager);

        entity = new Entity("Main Camera");
        entity->addComponent<FPSCamera>(glm::vec3(0), 4.781f, -0.549f);
        auto& cam = entity->getComponent<FPSCamera>().usingImGuiWindow = true;
        entity->getComponent<Transform>().position = glm::vec3(17, 11, -.01f);
        G_GetManager()->addEntity(entity);
        
        entity = new Entity("Afrodta Statue");
        G_GetManager()->addEntity(entity);

        // 0 = albedo
        // 1 = normal
        // 2 = metalic
        // 3 = roughness
        // 4 = ambient 

        entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");
        
        entity->addComponent<Mesh>("Afrodta_Statue/Rz_123_Afrodyta_z_Melos_2.obj");
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

        //entity->addComponent<script_simplebehaviours>();
        //entity->getComponent<script_simplebehaviours>().SetRotateActive(true);
        //entity->getComponent<script_simplebehaviours>().SetRotateAxis(false, true, false);

        //entity = new Entity("StanfordDragon");
        //G_GetManager()->addEntity(entity);

        //entity->addComponent<Mesh>("TheStanfordDragon_83k.obj");
        //entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

        //entity->getComponent<ShaderProgram>().AddTexturePath("");
        //entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Normal.png");
        //entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Metallic.png");
        //entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Roughness.png");
        //entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_AO.png");
        //entity->getComponent<ShaderProgram>().LoadTextures();

        //entity->getComponent<ShaderProgram>().SetColour(glm::vec4(16.0f/255.0f,189.0f / 255.0f,163.0f / 255.0f,255.0f / 255.0f),"albedo_color");
        //entity->getComponent<ShaderProgram>().SetFloat(2.5f,"normal_scalar");
        //entity->getComponent<ShaderProgram>().SetFloat(0.124f,"metallic_scalar");
        //entity->getComponent<ShaderProgram>().SetFloat(0.89f,"roughness_scalar");
        //entity->getComponent<ShaderProgram>().SetFloat(3.9f,"occlusion_scalar");

        ////entity->getComponent<Transform>().position = glm::vec3(9.0f,4.0f,0.0f);
        //entity->getComponent<Transform>().position = glm::vec3(0.0f);
        //entity->getComponent<Transform>().rotation = glm::vec3(-180, 65, -180);
        //entity->getComponent<Transform>().scale = glm::vec3(50.0f);

        entity = new Entity("Ground");
        G_GetManager()->addEntity(entity);

        entity->addComponent<Mesh>("cube.obj");
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
        entity->getComponent<Transform>().position = glm::vec3(0, -1, 0);
        entity->getComponent<Transform>().scale = glm::vec3(10, 1, 10);


        entity = new Entity("Directional Light");
        G_GetManager()->addEntity(entity);
        entity->getComponent<Transform>().position = glm::vec3(8.0f,4.5f,-1.93f);
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

        //std::cout << "Amount Of Depth Textures : " << depthTexturesThisFrame.size() << std::endl;

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

