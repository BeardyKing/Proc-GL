#include "test_post_processing.h"
#include "../Engine/Engine_UtilityFunctions.h"

using namespace test;

test_post_processing::test_post_processing(){

    editor = new EditorGUI;
    auto* manager = new EntityManager;
    G_SetManager(manager);

    create_camera();
    create_sky_box();
    create_water();
    create_point_light_shadow();
    create_point_light();
    create_post_processing();

    initialise_frame_buffers();
}

test_post_processing ::~test_post_processing() {
    delete entity;
    delete editor;
    delete camera;
    delete e_skybox;

    fbo.UnBind();
    glViewport(0, 0, G_GetWindowWidth(), G_GetWindowHeight());
}

void test_post_processing::OnUpdate(double deltaTime) {
    G_GetManager()->OnUpdate(deltaTime);
}

void test_post_processing::OnRender() {

    update_frame_buffer_size();

    render_texture_shadow_map();
    render_texture_no_transparent();

    draw();

    post_processing_depth_of_field();
    post_processing_vignette();
    post_processing_flip();

}

void test_post_processing::draw() {
    glEnable(GL_DEPTH_TEST);  // We still want depth test
    glDepthFunc(GL_LEQUAL);   // EQUAL should work, too. (Only draw pixels if they are the closest ones)

    fbo.Bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    G_GetManager()->OnRender();

    fbo.UnBind();
}

void test_post_processing::OnExit() {
    G_GetManager()->OnExit();
}

void test_post_processing::update_frame_buffer_size(){
    if (editor->windowSizeChangeFlag) {
        editor->UpdateFrameBufferTextureSize(fbo.GetRenderBuffer());
        fbo.UpdateFrameBufferTextureSize(camera->ImGuiWindowSize.x, camera->ImGuiWindowSize.y);
        fbo_render_pass.UpdateFrameBufferTextureSize(camera->ImGuiWindowSize.x, camera->ImGuiWindowSize.y);
        fbo_post_process.UpdateFrameBufferTextureSize(camera->ImGuiWindowSize.x, camera->ImGuiWindowSize.y);
        fbo_post_process2.UpdateFrameBufferTextureSize(camera->ImGuiWindowSize.x, camera->ImGuiWindowSize.y);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return;
    }

    depthTexturesThisFrame.clear();
}

void test_post_processing::render_texture_shadow_map(){
    for (auto& buffer : depthBuffers) {
        buffer.Bind();
        G_GetManager()->OnRender();
        G_SetShadowMap(buffer.GetDepthBuffer()); // this needs to become list/array
        depthTexturesThisFrame.emplace_back(buffer.GetDepthBuffer());
        buffer.UnBind();
    }
}

void test_post_processing::render_texture_no_transparent(){
    fbo_render_pass.Bind(); // DISABLED FOR SSR TESTING
    if (e_water != nullptr) { // TODO change to disable all objects with transparent tag
        e_water->isActive(false);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);  // We want depth test !
    glDepthFunc(GL_LESS);     // We want to get the nearest pixels

    G_GetManager()->OnRender();
    G_SetRenderPass(fbo_render_pass);
    if (e_water != nullptr) {
        e_water->isActive(true);
    }
    fbo_render_pass.UnBind();
}




void test_post_processing::OnImGuiRender() {

    #pragma region ImGui_Editor_DEBUG


    if (ImGui::IsKeyReleased('P')) { show_editor = !show_editor; }


    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    if (show_editor == true) {
    ImGui::Begin("Debug_FrameBuffers");

        int counter = 0;
        for (auto& buffer : depthTexturesThisFrame) {
            std::string name = "DEPTH TEXTURE ";
            name.append(std::to_string(counter));
            if (ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
                int adjustedWidth = ImGui::GetContentRegionAvailWidth() * 1024 / 1024;
                ImGui::Image((void*)buffer, ImVec2(ImGui::GetContentRegionAvailWidth(), adjustedWidth));
            }
            counter++;
        }

        if (ImGui::CollapsingHeader("FBO_POST_PROCESS", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
            int adjustedWidth = ImGui::GetContentRegionAvailWidth() * 1024 / 1024;
            ImGui::Image((void*)fbo_post_process.GetRenderBuffer(), ImVec2(ImGui::GetContentRegionAvailWidth(), adjustedWidth));
        }

        if (ImGui::CollapsingHeader("FBO_DEPTH_For_ImGui", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
            int adjustedWidth = ImGui::GetContentRegionAvailWidth() * 1024 / 1024;
            ImGui::Image((void*)G_GetRenderPass().GetDepthBuffer(), ImVec2(ImGui::GetContentRegionAvailWidth(), adjustedWidth));
        }

        if (ImGui::CollapsingHeader("FBO_For_ImGui", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
            int adjustedWidth = ImGui::GetContentRegionAvailWidth() * 1024 / 1024;
            ImGui::Image((void*)fbo.GetRenderBuffer(), ImVec2(ImGui::GetContentRegionAvailWidth(), adjustedWidth));
        }


        ImGui::End();
    }

    #pragma endregion

    editor->RenderScene(fbo_post_process2.GetRenderBuffer());

    #pragma region ImGui_Editor

    if (show_editor){
		editor->RenderHierarchy();
		editor->RenderProject();
		editor->RenderConsole();
		editor->RenderMainMenuBar();
		editor->RenderActiveInspector();
    }
    #pragma endregion
}

void test_post_processing::post_processing_flip() {

    fbo_post_process2.Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    post_processing->isActive(true);
    post_processing->getComponent<ShaderProgram>().SetInt(0, "isDepthOfField");
    post_processing->getComponent<ShaderProgram>().SetInt(0, "isVignette");
    post_processing->getComponent<ShaderProgram>().SetInt(0, "isColorCorrection");
    post_processing->getComponent<ShaderProgram>().SetInt(1, "isFlipImage");

    post_processing->getComponent<ShaderProgram>().SetRenderTexture(fbo_post_process.GetRenderBuffer());
    post_processing->getComponent<ShaderProgram>().SetRenderTexture(fbo.GetDepthBuffer());

    post_processing->getComponent<ShaderProgram>().SetRenderTexture(fbo.GetRenderBuffer());
    post_processing->getComponent<ShaderProgram>().SetRenderTexture(fbo_post_process.GetRenderBuffer());
    post_processing->OnRender();
    post_processing->isActive(false);
    post_processing->getComponent<ShaderProgram>().SetInt(0, "blendPass");
    fbo_post_process2.UnBind();

}

void test_post_processing::post_processing_depth_of_field() {

    // blur 1
    fbo_post_process.Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    post_processing->isActive(true);
    post_processing->getComponent<ShaderProgram>().SetInt(1, "isDepthOfField");
    post_processing->getComponent<ShaderProgram>().SetInt(0, "isVignette");
    post_processing->getComponent<ShaderProgram>().SetInt(0, "isColorCorrection");
    post_processing->getComponent<ShaderProgram>().SetInt(0, "isFlipImage");


    post_processing->getComponent<ShaderProgram>().SetInt(1, "isVertical");
    post_processing->getComponent<ShaderProgram>().SetInt(0, "renderPassCount");
    post_processing->getComponent<ShaderProgram>().SetRenderTexture(fbo.GetRenderBuffer());
    post_processing->getComponent<ShaderProgram>().SetRenderTexture(fbo.GetDepthBuffer());
    post_processing->OnRender();
    post_processing->isActive(false);
    fbo_post_process.UnBind();

    fbo_post_process2.Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    post_processing->isActive(true);
    post_processing->getComponent<ShaderProgram>().SetInt(0, "isVertical");
    post_processing->getComponent<ShaderProgram>().SetRenderTexture(fbo_post_process.GetRenderBuffer());
    post_processing->getComponent<ShaderProgram>().SetRenderTexture(fbo.GetDepthBuffer());
    post_processing->OnRender();
    post_processing->isActive(false);
    fbo_post_process2.UnBind();
    // blur 2
    fbo_post_process.Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    post_processing->isActive(true);
    post_processing->getComponent<ShaderProgram>().SetInt(1, "isVertical");
    post_processing->getComponent<ShaderProgram>().SetInt(1, "renderPassCount");
    post_processing->getComponent<ShaderProgram>().SetRenderTexture(fbo_post_process2.GetRenderBuffer());
    post_processing->getComponent<ShaderProgram>().SetRenderTexture(fbo.GetDepthBuffer());
    post_processing->OnRender();
    post_processing->isActive(false);
    fbo_post_process.UnBind();

    fbo_post_process2.Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    post_processing->isActive(true);
    post_processing->getComponent<ShaderProgram>().SetInt(0, "isVertical");
    post_processing->getComponent<ShaderProgram>().SetRenderTexture(fbo_post_process.GetRenderBuffer());
    post_processing->getComponent<ShaderProgram>().SetRenderTexture(fbo.GetDepthBuffer());
    post_processing->OnRender();
    post_processing->isActive(false);
    fbo_post_process2.UnBind();
    // blur 3
    fbo_post_process.Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    post_processing->isActive(true);
    post_processing->getComponent<ShaderProgram>().SetInt(1, "isVertical");
    post_processing->getComponent<ShaderProgram>().SetInt(2, "renderPassCount");
    post_processing->getComponent<ShaderProgram>().SetRenderTexture(fbo_post_process2.GetRenderBuffer());
    post_processing->getComponent<ShaderProgram>().SetRenderTexture(fbo.GetDepthBuffer());
    post_processing->OnRender();
    post_processing->isActive(false);
    fbo_post_process.UnBind();

    fbo_post_process2.Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    post_processing->isActive(true);
    post_processing->getComponent<ShaderProgram>().SetInt(0, "isVertical");
    post_processing->getComponent<ShaderProgram>().SetRenderTexture(fbo_post_process.GetRenderBuffer());
    post_processing->getComponent<ShaderProgram>().SetRenderTexture(fbo.GetDepthBuffer());
    post_processing->OnRender();
    post_processing->isActive(false);
    fbo_post_process2.UnBind();
}

void test_post_processing::post_processing_vignette() {
#pragma region POST_PROCESS_VEGNETTE

    fbo_post_process.Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    post_processing->isActive(true);
    post_processing->getComponent<ShaderProgram>().SetInt(0, "isDepthOfField");
    post_processing->getComponent<ShaderProgram>().SetInt(1, "isVignette");
    post_processing->getComponent<ShaderProgram>().SetInt(0, "isColorCorrection");
    post_processing->getComponent<ShaderProgram>().SetInt(0, "isFlipImage");

    post_processing->getComponent<ShaderProgram>().SetRenderTexture(fbo_post_process2.GetRenderBuffer());
    post_processing->getComponent<ShaderProgram>().SetRenderTexture(fbo.GetDepthBuffer());

    post_processing->getComponent<ShaderProgram>().SetRenderTexture(fbo.GetRenderBuffer());
    post_processing->getComponent<ShaderProgram>().SetRenderTexture(fbo_post_process2.GetRenderBuffer());
    post_processing->OnRender();
    post_processing->isActive(false);
    post_processing->getComponent<ShaderProgram>().SetInt(0, "blendPass");
    fbo_post_process.UnBind();

#pragma endregion

}

void test_post_processing::initialise_frame_buffers() {
    DepthBuffer d(1024 * 4, 1024 * 4);
    depthBuffers.emplace_back(d);

    fbo.GenerateFrameBuffer(editor->lastFrameWindowSize.x, editor->lastFrameWindowSize.y);
    fbo_render_pass.GenerateFrameBuffer(editor->lastFrameWindowSize.x, editor->lastFrameWindowSize.y);
    fbo_post_process.GenerateFrameBuffer(editor->lastFrameWindowSize.x, editor->lastFrameWindowSize.y);
    fbo_post_process2.GenerateFrameBuffer(editor->lastFrameWindowSize.x, editor->lastFrameWindowSize.y);

}

void test_post_processing::create_post_processing()
{
    entity = new Entity("post_processing_object");
    entity->addComponent<ShaderProgram>("Shaders/PostProcessing/post_processing.vert", "Shaders/PostProcessing/post_processing.frag", "Uniform_Post_Processing");
    entity->getComponent<ShaderProgram>().LoadTextures();
    entity->addComponent<Mesh>();
    entity->getComponent<Mesh>().Generate_primitive_Plane();
    entity->getComponent<ShaderProgram>().SetFloat(10.0f, "blurStrength_0");
    entity->getComponent<ShaderProgram>().SetFloat(2.147f, "blurStrength_1");
    entity->getComponent<ShaderProgram>().SetFloat(6.712f, "blurStrength_2");
    post_processing = entity;
    post_processing->isActive(false);
    G_GetManager()->addEntity(entity);
}

void test_post_processing::create_point_light()
{
    entity = new Entity("Point Light1");
    entity->getComponent<Transform>().position = glm::vec3(-20, 15, -15);
    entity->addComponent<LightObject>();
    entity->getComponent<LightObject>().lightType = LightObject::Point;
    entity->getComponent<LightObject>().far_plane = 200;
    entity->getComponent<LightObject>().near_plane = 43;
    entity->addComponent<script_simplebehaviours>();

    std::vector<Entity*> lights = G_GetManager()->FindLights();
    G_GetManager()->addEntity(entity);
}

void test_post_processing::create_point_light_shadow() {
    entity = new Entity("Point Light Shadow");
    G_GetManager()->addEntity(entity);
    entity->getComponent<Transform>().position = glm::vec3(51, 64, 37);
    entity->addComponent<LightObject>();
    entity->getComponent<LightObject>().lightType = LightObject::Point;
    entity->getComponent<LightObject>().lightIntensity = 4.6f;
    entity->getComponent<LightObject>().far_plane = 147;
    entity->getComponent<LightObject>().near_plane = 39.45f;
    entity->getComponent<LightObject>().lookAtTarget = glm::vec3(-19, 0, -16);
    entity->getComponent<LightObject>().v4lightProjection = glm::vec4(-70, 70, -70, 70);
    entity->getComponent<LightObject>().shadowIntensity = -0.2f;
    entity->addComponent<script_simplebehaviours>();

}

void test_post_processing::create_water() {
    entity = new Entity("Water");

    entity->addComponent<ShaderProgram>("Shaders/Water/Standard_Water.vert", "Shaders/Water/Standard_Water.frag", "Uniform_Standard_Water");

    entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);
    // 0 = albedo
    // 1 = normal
    // 2 = metallic
    // 3 = roughness
    // 4 = ambient 
    // 4 = DuDv map
    entity->addComponent<Mesh>("8502_Assets/water/water.obj");
    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/water/");
    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/water/Normals 1.png");
    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/water/Water_001_DISP.png");
    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/water/Water_001_SPEC.jpg");
    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/water/");
    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/water/dudv_halisavkis.png"); // has strange artifact but is seamless
    entity->getComponent<ShaderProgram>().LoadTextures();

    //entity->getComponent<ShaderProgram>().SetColour(glm::vec4(91.0f/255.0f, 171.0f /255.0f, 255.0f /255.0f, 255.0f /255.0f), "albedo_color");
    entity->getComponent<ShaderProgram>().SetColour(glm::vec4(1.0f), "albedo_color");
    entity->getComponent<ShaderProgram>().SetFloat(1.078f, "normal_scalar");
    entity->getComponent<ShaderProgram>().SetFloat(0.0f, "metallic_scalar");
    entity->getComponent<ShaderProgram>().SetFloat(0.058f, "roughness_scalar");
    entity->getComponent<ShaderProgram>().SetFloat(4.595f, "occlusion_scalar");
    entity->getComponent<ShaderProgram>().SetFloat(7.71f, "waterDepthBlend");
    entity->getComponent<ShaderProgram>().SetFloat(0.024f, "DuDv_scalar");
    entity->getComponent<ShaderProgram>().SetVec2(glm::vec2(0.05), "scroll_amount");
    entity->getComponent<ShaderProgram>().SetTextureScale(glm::vec2(110, 90));
    e_water = entity;

    G_GetManager()->addEntity(entity);
}

void test_post_processing::create_sky_box() {
    std::vector<std::string> faces = {
        "skybox/right.jpg",
        "skybox/left.jpg",
        "skybox/top.jpg",
        "skybox/bottom.jpg",
        "skybox/front.jpg",
        "skybox/back.jpg"
    };

    e_skybox = new Entity("Skybox");

    e_skybox->addComponent<Mesh>("objectDefaults/cube.obj");
    e_skybox->addComponent<SkyBox>(faces);
    e_skybox->addComponent<ShaderProgram>("Shaders/skybox/skybox.vert", "Shaders/skybox/skybox.frag", "Uniform_Skybox");

    G_GetManager()->addEntity(e_skybox);
}
void test_post_processing::create_camera() {
    entity = new Entity("Main Camera");
    entity->addComponent<FPSCamera>();
    auto& cam = entity->getComponent<FPSCamera>().usingImGuiWindow = true;
    entity->getComponent<Transform>().position = glm::vec3(35, 9, 6);
    entity->getComponent<FPSCamera>().Rotate(260.0f, -45.0f);
    entity->getComponent<FPSCamera>().SetZNear(0.5f);
    entity->addComponent<script_simplebehaviours>();
    entity->getComponent<script_simplebehaviours>().SetCameraMoveActive(false);
    camera = &entity->getComponent<FPSCamera>();

    G_GetManager()->addEntity(entity);
}
