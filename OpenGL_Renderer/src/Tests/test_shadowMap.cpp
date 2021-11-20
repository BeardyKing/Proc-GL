#include "test_shadowMap.h"
#include "../Engine/Engine_UtilityFunctions.h"
#include "../Components/Terrain.h"
#include "../Engine/Anim_Mesh.h"
#include "../Engine/Mesh_Material.h"


RenderPass fbo_render_pass;
RenderPass G_GetRenderPass() {return fbo_render_pass;}
void G_SetRenderPass(RenderPass tex) { fbo_render_pass = tex;}

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
        entity->addComponent<FPSCamera>();
        auto& cam = entity->getComponent<FPSCamera>().usingImGuiWindow = true;
        entity->getComponent<Transform>().position = glm::vec3(35, 9, 6);
        entity->getComponent<FPSCamera>().Rotate(260.0f,-45.0f);
        entity->getComponent<FPSCamera>().SetZNear(0.5f);
        G_GetManager()->addEntity(entity);
        camera = &entity->getComponent<FPSCamera>();

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
        e_skybox->addComponent<SkyBox>(faces);
        e_skybox->addComponent<ShaderProgram>("Shaders/skybox/skybox.vert", "Shaders/skybox/skybox.frag","Uniform_Skybox");
        
    #pragma endregion

    #pragma region Terrain

        entity = new Entity("Terrain");
        G_GetManager()->addEntity(entity);

        entity->addComponent<Terrain>();
        entity->addComponent<Mesh>();
        entity->getComponent<Mesh>().LoadTerrain(entity->getComponent<Terrain>().GetVertexData());
        
        entity->addComponent<ShaderProgram>("Shaders/Terrain/Standard_Terrain.vert", "Shaders/Terrain/Standard_Terrain.frag", "Uniform_Standard_Terrain");
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

#pragma region AnimMesh
		entity = new Entity("Animated Mesh");
		G_GetManager()->addEntity(entity);
        entity->addComponent<ShaderProgram>("Shaders/anim_model/anim_model.vert", "Shaders/anim_model/anim_model.frag", "Uniform_Standard_Lit");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_coins/gold_coin_color.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_coins/gold_coin_color.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_coins/gold_coin_color.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_coins/gold_coin_color.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_coins/gold_coin_color.png");
        entity->getComponent<ShaderProgram>().LoadTextures();
        entity->addComponent<Anim_Mesh>();
        entity->getComponent<Anim_Mesh>().LoadFromMeshFile("8502_Assets/animation/Role_T.msh");
        entity->addComponent<MeshMaterial>("8502_Assets/animation/Role_T.mat");
        //entity->getComponent<Anim_Mesh>().GenerateQuad();

#pragma endregion


    #pragma region TODO Write bespoke shaders


    //#pragma region gold_coins
    //    entity = new Entity("gold_coins");
    //    G_GetManager()->addEntity(entity);

    //    entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

    //    entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

    //    // 0 = albedo
    //    // 1 = normal
    //    // 2 = metalic
    //    // 3 = roughness
    //    // 4 = ambient 

    //    entity->addComponent<Mesh>("8502_Assets/gold_coins/gold_coins.obj");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_coins/gold_coin_color.png");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_coins/gold_coin_normals.png");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_coins/gold_coin_Specular.png");
    //    entity->getComponent<ShaderProgram>().LoadTextures();
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "normal_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "metallic_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "roughness_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "occlusion_scalar");

    //#pragma endregion

    //#pragma region bars
    //    entity = new Entity("gold_bars");
    //    G_GetManager()->addEntity(entity);

    //    entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

    //    entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

    //    // 0 = albedo
    //    // 1 = normal
    //    // 2 = metalic
    //    // 3 = roughness
    //    // 4 = ambient 

    //    entity->addComponent<Mesh>("8502_Assets/gold_bars/gold_bars.obj");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_bars/gold_bar_color.jpg");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_bars/gold_bar_normals.jpg");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_bars/gold_bar_Specular.jpg");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_bars/gold_bar_Specular.jpg");
    //    entity->getComponent<ShaderProgram>().LoadTextures();
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "normal_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "metallic_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "roughness_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "occlusion_scalar");

    //#pragma endregion

    //#pragma region dock

    //    entity = new Entity("ancient_wall");
    //    G_GetManager()->addEntity(entity);

    //    entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

    //    entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

    //    entity->addComponent<Mesh>("8502_Assets/dock/dock.obj");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/dock/T_wood_bridge_BC.tga");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/dock/T_wood_bridge_N.tga");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/dock/T_wood_bridge_Metalic_AO.tga");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("T_wood_bridge_Metalic_AO.tga");
    //    entity->getComponent<ShaderProgram>().LoadTextures();
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "normal_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "metallic_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "roughness_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "occlusion_scalar");

    //#pragma endregion

    //#pragma region boat
    //    entity = new Entity("boat");
    //    G_GetManager()->addEntity(entity);

    //    entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

    //    entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

    //    entity->addComponent<Mesh>("8502_Assets/boat/boat.obj");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/boat/Boat_Wood_BC.jpg");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/boat/Boat_Wood_Normal.jpg");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/boat/Boat_Wood_MS.jpg");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/boat/Boat_Wood_MS.jpg");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/boat/Boat_Wood_AO.jpg");
    //    entity->getComponent<ShaderProgram>().LoadTextures();
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "normal_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "metallic_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "roughness_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "occlusion_scalar");

    //    entity = new Entity("boat_paddles");
    //    G_GetManager()->addEntity(entity);

    //    entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

    //    entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

    //    entity->addComponent<Mesh>("8502_Assets/boat/boat_paddles.obj");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/boat/Paddle_Tarp_BC.jpg");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/boat/Paddle_Tarp_N.jpg");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/boat/Boat_Wood_MS.jpg");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/boat/Boat_Wood_MS.jpg");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/boat/Paddle_Tarp_AO.jpg");
    //    entity->getComponent<ShaderProgram>().LoadTextures();
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "normal_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "metallic_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "roughness_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "occlusion_scalar");

    //#pragma endregion

    //#pragma region ancient_wall

    //    entity = new Entity("ancient_wall");
    //    G_GetManager()->addEntity(entity);

    //    entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

    //    entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

    //    entity->addComponent<Mesh>("8502_Assets/ancient_wall/ancient_wall.obj");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/ancient_wall/ancient_wall_BaseColor.jpg");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/ancient_wall/ancient_wall_Normal.jpg");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/ancient_wall/ancient_wall_OcclusionRoughnessMetallic.jpg");        
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/ancient_wall/ancient_wall_Roughness.jpg");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/ancient_wall/ancient_wall_AO.jpg");
    //    entity->getComponent<ShaderProgram>().LoadTextures();
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "normal_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "metallic_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "roughness_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "occlusion_scalar");
    //#pragma endregion

    //#pragma region gold_columns
    //    entity = new Entity("gold_columns");
    //    G_GetManager()->addEntity(entity);

    //    entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

    //    entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

    //    // 0 = albedo
    //    // 1 = normal
    //    // 2 = metalic
    //    // 3 = roughness
    //    // 4 = ambient 

    //    entity->addComponent<Mesh>("8502_Assets/gold_columns/gold_columns.obj");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_columns/gold_coin_color.png");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_columns/gold_coin_normals.png");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_columns/gold_coin_Specular.png");
    //    entity->getComponent<ShaderProgram>().LoadTextures();
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "normal_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "metallic_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "roughness_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "occlusion_scalar");

    //#pragma endregion

    //#pragma region Vases

    //#pragma region Vase_Amphora
    //    entity = new Entity("Vase_Amphora");
    //    G_GetManager()->addEntity(entity);

    //    entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

    //    entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

    //    entity->addComponent<Mesh>("8502_Assets/Vases/railing_temp.obj");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Vases/Vase_Amphora_AlbedoTransparency.png");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Vases/Vase_Amphora_Normal.png");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("");         
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Vases/Vase_Amphora_MetallicSmoothness.png");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Vases/");
    //    entity->getComponent<ShaderProgram>().LoadTextures();
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "normal_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "metallic_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "roughness_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "occlusion_scalar");

    //#pragma endregion

    //#pragma region Vase_Hydria
    //    entity = new Entity("Vase_Hydria");
    //    G_GetManager()->addEntity(entity);

    //    entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

    //    entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

    //    entity->addComponent<Mesh>("8502_Assets/Vases/Vase_Hydria/Vase_Hydria.obj");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Vases/Vase_Hydria/Vase_Hydria_AlbedoTransparency.png");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Vases/Vase_Hydria/Vase_Hydria_Normal.png");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Vases/Vase_Hydria/Vase_Hydria_MetallicSmoothness.png");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Vases/");
    //    entity->getComponent<ShaderProgram>().LoadTextures();
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "normal_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "metallic_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "roughness_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "occlusion_scalar");

    //#pragma endregion

    //#pragma region Vase_Amphora
    //    entity = new Entity("Vase_VoluteKrater");
    //    G_GetManager()->addEntity(entity);

    //    entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

    //    entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

    //    entity->addComponent<Mesh>("8502_Assets/Vases/Vase_VoluteKrater/Vase_VoluteKrater.obj");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Vases/Vase_VoluteKrater/Vase_VoluteKrater_AlbedoTransparency.png");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Vases/Vase_VoluteKrater/Vase_VoluteKrater_Normal.png");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Vases/Vase_VoluteKrater/Vase_VoluteKrater_MetallicSmoothness.png");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Vases/");
    //    entity->getComponent<ShaderProgram>().LoadTextures();
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "normal_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "metallic_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "roughness_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "occlusion_scalar");

    //#pragma endregion


    //#pragma endregion

   /* #pragma region island_rocks

        entity = new Entity("marble ground");
        G_GetManager()->addEntity(entity);

        entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

        entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

        entity->addComponent<Mesh>("8502_Assets/island_rocks/island_rocks.obj");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/island_rocks/T_beech_forest_stones_01_BC_SM.tga");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/island_rocks/T_beech_forest_stones_01_N.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/island_rocks/T_beech_forest_stones_01_DETAIL.tga");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/island_rocks/T_beech_forest_stones_01_MT_AO_SM.tga");
        entity->getComponent<ShaderProgram>().LoadTextures();
        entity->getComponent<ShaderProgram>().SetFloat(1.0f, "normal_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(1.0f, "metallic_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(1.0f, "roughness_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(1.0f, "occlusion_scalar");

    #pragma endregion*/

    //   
    ////TODO ALPHA CUTOFF SHADER FOR TREE // TODO fix backface culling on object

    //#pragma region Marble_Railings_TEMP
    //    entity = new Entity("Marble_Railings_TEMP");
    //    G_GetManager()->addEntity(entity);

    //    entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

    //    entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

    //    entity->addComponent<Mesh>("8502_Assets/railing_temp/railing_temp.obj");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_diff_1k.png");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_nor_1k.png");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_rough_1k.png");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_AO_1k.png");
    //    entity->getComponent<ShaderProgram>().LoadTextures();
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "normal_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "metallic_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "roughness_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "occlusion_scalar");
    //#pragma endregion

    //#pragma region Marble_Ground
    //    entity = new Entity("marble ground");
    //    G_GetManager()->addEntity(entity);

    //    entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");
    //    
    //    entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

    //    entity->addComponent<Mesh>("8502_Assets/marble_ground/Marble_Ground.obj");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_diff_1k.png");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_nor_1k.png");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_rough_1k.png");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_AO_1k.png");
    //    entity->getComponent<ShaderProgram>().LoadTextures();
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "normal_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "metallic_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "roughness_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "occlusion_scalar");
    //#pragma endregion

    //#pragma region Statue
    //    entity = new Entity("Afrodta Statue");
    //    G_GetManager()->addEntity(entity);

    //    // 0 = albedo
    //    // 1 = normal
    //    // 2 = metalic
    //    // 3 = roughness
    //    // 4 = ambient 
    //    entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

    //    entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");
    //    
    //    entity->addComponent<Mesh>("8502_Assets/afrodta_statues/afrodta_Statues.obj");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/afrodta_statues/afrodta_Statues_albedo1k.jpg");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/afrodta_statues/afrodta_Statues_normal1k.jpg");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/afrodta_statues/Painted_metal_02_1K_Metallic.png");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/afrodta_statues/Painted_metal_02_1K_Roughness.png");
    //    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/afrodta_statues/Painted_metal_02_1K_AO.png");
    //    entity->getComponent<ShaderProgram>().LoadTextures();
    //    
    //    entity->getComponent<ShaderProgram>().SetFloat(0.864f, "normal_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(0.087f, "metallic_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(0.322f, "roughness_scalar");
    //    entity->getComponent<ShaderProgram>().SetFloat(4.6f, "occlusion_scalar");


    //#pragma region Tree_box_Temp_Shaders
    // 
    //entity = new Entity("Tree_Box");
    //G_GetManager()->addEntity(entity);

    //entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

    //entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

    //entity->addComponent<Mesh>("8502_Assets/tree_box/tree_box.obj");
    //entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_diff_1k.png");
    //entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_nor_1k.png");
    //entity->getComponent<ShaderProgram>().AddTexturePath("");
    //entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_rough_1k.png");
    //entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_AO_1k.png");
    //entity->getComponent<ShaderProgram>().LoadTextures();
    //entity->getComponent<ShaderProgram>().SetFloat(1.0f, "normal_scalar");
    //entity->getComponent<ShaderProgram>().SetFloat(1.0f, "metallic_scalar");
    //entity->getComponent<ShaderProgram>().SetFloat(1.0f, "roughness_scalar");
    //entity->getComponent<ShaderProgram>().SetFloat(1.0f, "occlusion_scalar");
    //#pragma endregion

    //#pragma region Tree 
    //entity = new Entity("tree");
    //G_GetManager()->addEntity(entity);

    //entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

    //entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

    //entity->addComponent<Mesh>("8502_Assets/tree/tree.obj");
    //entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/tree/T_beech_atlas_BC.tga");
    //entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/tree/T_beech_atlas_N.png");
    //entity->getComponent<ShaderProgram>().AddTexturePath("");
    //entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/tree/T_beech_atlas_N.tga");
    //entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/tree/");
    //entity->getComponent<ShaderProgram>().LoadTextures();
    //entity->getComponent<ShaderProgram>().SetFloat(1.0f, "normal_scalar");
    //entity->getComponent<ShaderProgram>().SetFloat(1.0f, "metallic_scalar");
    //entity->getComponent<ShaderProgram>().SetFloat(1.0f, "roughness_scalar");
    //entity->getComponent<ShaderProgram>().SetFloat(1.0f, "occlusion_scalar");
    //#pragma endregion

    //#pragma region Tree_Glass_box

    //entity = new Entity("glass_tree_box");
    //G_GetManager()->addEntity(entity);

    //// 0 = albedo
    //// 1 = normal
    //// 2 = metalic
    //// 3 = roughness
    //// 4 = ambient 
    //entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

    //entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

    //entity->addComponent<Mesh>("8502_Assets/tree_glass_box/tree_glass_box.obj");
    //entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/tree_glass_box/glass_basemap.png");
    //entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/tree_glass_box/glass_normal.png");
    //entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/tree_glass_box/");
    //entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/tree_glass_box/");
    //entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/tree_glass_box/");
    //entity->getComponent<ShaderProgram>().LoadTextures();

    //entity->getComponent<ShaderProgram>().SetFloat(0.864f, "normal_scalar");
    //entity->getComponent<ShaderProgram>().SetFloat(0.087f, "metallic_scalar");
    //entity->getComponent<ShaderProgram>().SetFloat(0.322f, "roughness_scalar");
    //entity->getComponent<ShaderProgram>().SetFloat(4.6f, "occlusion_scalar");

    //entity->isActive(false);

    //#pragma endregion
    //#pragma endregion

entity = new Entity("SPHERE");
	G_GetManager()->addEntity(entity);

	// 0 = albedo
	// 1 = normal
	// 2 = metalic
	// 3 = roughness
	// 4 = ambient 
	entity->getComponent<Transform>().position = glm::vec3(0, -12, 0);
	entity->getComponent<Transform>().scale = glm::vec3(7);

	entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

	entity->addComponent<Mesh>("sphere64_64.obj");
	entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Base_Color.png");
	entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Normal.png");
	entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Metallic.png");
	entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Roughness.png");
	entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_AO.png");
	entity->getComponent<ShaderProgram>().LoadTextures();

	entity->getComponent<ShaderProgram>().SetFloat(0.342f, "normal_scalar");
	entity->getComponent<ShaderProgram>().SetFloat(1.0f, "metallic_scalar");
	entity->getComponent<ShaderProgram>().SetFloat(1.177f, "roughness_scalar");
	entity->getComponent<ShaderProgram>().SetFloat(0.0f, "occlusion_scalar");


    #pragma endregion

    #pragma region Water
    // 0 = albedo
    // 1 = normal
    // 2 = metalic
    // 3 = roughness
    // 4 = ambient 
        entity = new Entity("Water");
        G_GetManager()->addEntity(entity);

        entity->addComponent<ShaderProgram>("Shaders/Water/Standard_Water.vert", "Shaders/Water/Standard_Water.frag", "Uniform_Standard_Water");

        entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

        entity->addComponent<Mesh>("8502_Assets/water/water.obj");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/water/");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/water/Normals 1.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/water/Water_001_DISP.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/water/Water_001_SPEC.jpg");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/water/");
        entity->getComponent<ShaderProgram>().LoadTextures();

        //entity->getComponent<ShaderProgram>().SetColour(glm::vec4(91.0f/255.0f, 171.0f /255.0f, 255.0f /255.0f, 255.0f /255.0f), "albedo_color");
        entity->getComponent<ShaderProgram>().SetColour(glm::vec4(1.0f), "albedo_color");
        entity->getComponent<ShaderProgram>().SetFloat(0.630f, "normal_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(0.460f, "metallic_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(0.162f, "roughness_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(1.875f, "occlusion_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(3.0f, "waterDepthBlend");
        entity->getComponent<ShaderProgram>().SetTextureScale(glm::vec2(70));
        e_water = entity;
#pragma endregion

    #pragma region point_Light

        entity = new Entity("Point Light Shadow");
		G_GetManager()->addEntity(entity);
		entity->getComponent<Transform>().position = glm::vec3(44, 40, 72);
		entity->addComponent<LightObject>();
		entity->getComponent<LightObject>().lightType = LightObject::Point;
		entity->getComponent<LightObject>().far_plane = 200;
		entity->getComponent<LightObject>().near_plane = 43;
		entity->getComponent<LightObject>().v4lightProjection = glm::vec4(-45, 45, -45, 45);
		entity->addComponent<script_simplebehaviours>();

		entity = new Entity("Point Light1");
        G_GetManager()->addEntity(entity);
		entity->getComponent<Transform>().position = glm::vec3(-20, -10, -15);
        entity->addComponent<LightObject>();
        entity->getComponent<LightObject>().lightType = LightObject::Point;
        entity->getComponent<LightObject>().far_plane = 200;
        entity->getComponent<LightObject>().near_plane = 43;
        entity->addComponent<script_simplebehaviours>();

		entity = new Entity("Point Light2");
		G_GetManager()->addEntity(entity);
		entity->getComponent<Transform>().position = glm::vec3(10, -6, 9);
		entity->addComponent<LightObject>();
		entity->getComponent<LightObject>().lightType = LightObject::Point;
		entity->getComponent<LightObject>().far_plane = 200;
		entity->getComponent<LightObject>().near_plane = 43;
		entity->addComponent<script_simplebehaviours>();

        std::vector<Entity*> lights = manager->FindLights();

    #pragma endregion


    #pragma region Frame Buffer (imGUI Image)

        DepthBuffer d(1024 * 4, 1024 * 4);
        //DepthBuffer s(1024 * 4, 1024 * 4);
        depthBuffers.emplace_back(d);
        //depthBuffers.emplace_back(s);
        
        fbo.GenerateFrameBuffer(editor->lastFrameWindowSize.x, editor->lastFrameWindowSize.y);
        fbo_render_pass.GenerateFrameBuffer(editor->lastFrameWindowSize.x, editor->lastFrameWindowSize.y);

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
            fbo_render_pass.UpdateFrameBufferTextureSize(camera->ImGuiWindowSize.x, camera->ImGuiWindowSize.y);
            //editor->UpdateFrameBufferTextureSize(fbo_cam_depth.GetRenderBuffer());
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

        {
            fbo_render_pass.Bind(); // DISABLED FOR SSR TESTING
			if (e_water != nullptr) {
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

		glEnable(GL_DEPTH_TEST);  // We still want depth test
		glDepthFunc(GL_LEQUAL);   // EQUAL should work, too. (Only draw pixels if they are the closest ones)

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
                ImGui::Image((void*)buffer, ImVec2(ImGui::GetContentRegionAvailWidth(), adjustedWidth));
            }
            counter++;
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

        editor->RenderScene(fbo.GetRenderBuffer());
        editor->RenderHierarchy();
        editor->RenderProject();
        editor->RenderConsole();
        editor->RenderMainMenuBar();
        editor->RenderActiveInspector();
    }
}

