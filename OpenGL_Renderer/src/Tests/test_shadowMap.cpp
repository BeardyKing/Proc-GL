#include "test_shadowMap.h"
#include "../Engine/Engine_UtilityFunctions.h"
#include "../Components/Terrain.h"
#include "../Engine/Anim_Mesh.h"
#include "../Engine/Mesh_Material.h"
#include "../Engine/Mesh_Animator.h"


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
        entity->addComponent<script_simplebehaviours>();
        entity->getComponent<script_simplebehaviours>().SetCameraMoveActive(true);

        G_GetManager()->addEntity(entity);
        camera = &entity->getComponent<FPSCamera>();

        #pragma endregion
        
        #pragma region Cubemap / Skybox

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

        e_skybox->addComponent<Mesh>("objectDefaults/cube.obj");
        e_skybox->addComponent<SkyBox>(faces);
        e_skybox->addComponent<ShaderProgram>("Shaders/skybox/skybox.vert", "Shaders/skybox/skybox.frag","Uniform_Skybox");
        
        #pragma endregion

        #pragma region Terrain
		// Standard Terrain Layout // 0 = albedo // 1 = normal // 2 = metallic // 3 = roughness // 4 = ambient 

        entity = new Entity("Terrain");
        G_GetManager()->addEntity(entity);

        entity->addComponent<Terrain>();
        entity->addComponent<Mesh>();
        entity->getComponent<Mesh>().LoadTerrain(entity->getComponent<Terrain>().GetVertexData());
        
        entity->addComponent<ShaderProgram>("Shaders/Terrain/Standard_Terrain.vert", "Shaders/Terrain/Standard_Terrain.frag", "Uniform_Standard_Terrain");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Terrain/Sand_004_COLOR.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Terrain/Sand_004_NRM.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Terrain/Sand_004_DISP.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Terrain/Sand_004_SPEC.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Terrain/Sand_004_OCC.png");
        entity->getComponent<ShaderProgram>().LoadTextures();
        entity->getComponent<ShaderProgram>().SetColour(glm::vec4( 255.0f / 255.0f , 255.0f / 236.0f, 255.0f / 198.0f ,1.0), "albedo_color");
        entity->getComponent<ShaderProgram>().SetFloat(2.282f, "normal_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(0.0f, "metallic_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(0.365f, "roughness_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(6.713f, "occlusion_scalar");
        entity->getComponent<ShaderProgram>().SetTextureScale(glm::vec2(1, 1));

        entity->getComponent<Transform>().scale = glm::vec3(0.01f);
        entity->getComponent<Transform>().position = glm::vec3(-80,-25,-60);

        #pragma endregion

        #pragma region AnimMesh

		entity = new Entity("Animated Mesh");
		G_GetManager()->addEntity(entity);
        entity->addComponent<ShaderProgram>("Shaders/anim_model/anim_model.vert", "Shaders/anim_model/anim_model.frag", "Uniform_Standard_Lit");
        entity->addComponent<Anim_Mesh>();
        entity->getComponent<Anim_Mesh>().LoadFromMeshFile("8502_Assets/animation/Minotaur.msh");
        entity->addComponent<MeshMaterial>("8502_Assets/animation/Minotaur.mat");
        entity->addComponent<MeshAnimation>("8502_Assets/animation/Minotaur.anm");
        entity->getComponent<Transform>().position = glm::vec3(6.749f,-7.071f, -21.392);
        entity->getComponent<Transform>().scale = glm::vec3(0.07f);
        
        
        // MAT SETUP START
		for (int i = 0; i < entity->getComponent<Anim_Mesh>().GetSubMeshCount(); ++i) {
			const MeshMaterialEntry* matEntry = entity->getComponent<MeshMaterial>().GetMaterialForLayer(i);
			const string* filename = nullptr;
			matEntry->GetEntry("Diffuse", &filename);
			string path = "8502_Assets/animation/" + *filename;
            Texture2D* tex = new Texture2D();
            tex->LoadTexture(path.c_str(), true);
            entity->getComponent<Anim_Mesh>().textures.emplace_back(tex);
		}
        
        entity->getComponent<ShaderProgram>().LoadTextures();
        e_animated_mesh = entity;
        entity->isActive(true);

        #pragma endregion

        #pragma region gold_coins
		
        // Standard Lit Texture Layout // 0 = albedo // 1 = normal // 2 = metallic // 3 = roughness // 4 = ambient 

        entity = new Entity("gold_coins");
        G_GetManager()->addEntity(entity);

        entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

        entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

        entity->addComponent<Mesh>("8502_Assets/gold_coins/gold_coins.obj");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_coins/gold_coin_color.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_coins/gold_coin_normals.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("");
        entity->getComponent<ShaderProgram>().AddTexturePath("");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_coins/gold_coin_Specular.png");
        entity->getComponent<ShaderProgram>().LoadTextures();
		entity->getComponent<ShaderProgram>().SetFloat(3.672f, "albedo_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(1.07f, "normal_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(1.0f, "metallic_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(0.248f, "roughness_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(1.19f, "occlusion_scalar");

        #pragma endregion

        #pragma region bars
		
        // Standard Lit Texture Layout // 0 = albedo // 1 = normal // 2 = metallic // 3 = roughness // 4 = ambient 

        entity = new Entity("gold_bars");
        G_GetManager()->addEntity(entity);

        entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

        entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

        entity->addComponent<Mesh>("8502_Assets/gold_bars/gold_bars.obj");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_bars/gold_bar_color.jpg");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_bars/gold_bar_normals.jpg");
        entity->getComponent<ShaderProgram>().AddTexturePath("");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_bars/gold_bar_Specular.jpg");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_bars/gold_bar_Specular.jpg");
        entity->getComponent<ShaderProgram>().LoadTextures();
		entity->getComponent<ShaderProgram>().SetFloat(3.672f, "albedo_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(1.07f, "normal_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(1.0f, "metallic_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(0.248f, "roughness_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(1.19f, "occlusion_scalar");

        #pragma endregion

        #pragma region dock
		
        // Standard Lit Texture Layout // 0 = albedo // 1 = normal // 2 = metallic // 3 = roughness // 4 = ambient 

		entity = new Entity("Dock");
		G_GetManager()->addEntity(entity);

		entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

		entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

		entity->addComponent<Mesh>("8502_Assets/dock/dock.obj");
		entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/dock/T_wood_bridge_BC.png");
		entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/dock/T_wood_bridge_N.png");
		entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/dock/T_wood_bridge_Metalic_AO.png");
		entity->getComponent<ShaderProgram>().AddTexturePath("");
		entity->getComponent<ShaderProgram>().AddTexturePath("T_wood_bridge_Metalic_AO.png");
		entity->getComponent<ShaderProgram>().LoadTextures();

		entity->getComponent<ShaderProgram>().SetColour(glm::vec4(255.0f / 255.0f, 202.0f /255.0f , 183.0f / 255.0f, 255.0f / 255.0f), "albedo_color");
		entity->getComponent<ShaderProgram>().SetFloat(1.64f, "albedo_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(0.79f, "normal_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(0.0f, "metallic_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(0.362f, "roughness_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(0.905f, "occlusion_scalar");
		

        #pragma endregion

        #pragma region boat

		// Standard Lit Texture Layout // 0 = albedo // 1 = normal // 2 = metallic // 3 = roughness // 4 = ambient 

        entity = new Entity("boat");
        G_GetManager()->addEntity(entity);

        entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

		entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

        entity->addComponent<Mesh>("8502_Assets/boat/boat.obj");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/boat/Boat_Wood_BC.jpg");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/boat/Boat_Wood_Normal.jpg");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/boat/Boat_Wood_MS.jpg");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/boat/Boat_Wood_MS.jpg");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/boat/Boat_Wood_AO.jpg");
        entity->getComponent<ShaderProgram>().LoadTextures();
		entity->getComponent<ShaderProgram>().SetFloat(1.0f, "normal_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(1.0f, "metallic_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(1.0f, "roughness_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(1.0f, "occlusion_scalar");

        entity = new Entity("boat_paddles");
        G_GetManager()->addEntity(entity);

        entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

        entity->getComponent<Transform>().position = glm::vec3(30.1f, -141.0f, 24.5f);
        entity->getComponent<Transform>().rotation = glm::vec3(-16, 12, 1);

        entity->addComponent<Mesh>("8502_Assets/boat/boat_paddles.obj");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/boat/Paddle_Tarp_BC.jpg");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/boat/Paddle_Tarp_N.jpg");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/boat/Boat_Wood_MS.jpg");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/boat/Boat_Wood_MS.jpg");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/boat/Paddle_Tarp_AO.jpg");
        entity->getComponent<ShaderProgram>().LoadTextures();
        entity->getComponent<ShaderProgram>().SetFloat(1.0f, "normal_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(1.0f, "metallic_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(1.0f, "roughness_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(1.0f, "occlusion_scalar");

        #pragma endregion

        #pragma region ancient_wall

		// Standard Lit Texture Layout // 0 = albedo // 1 = normal // 2 = metallic // 3 = roughness // 4 = ambient 

        entity = new Entity("ancient_wall");
        G_GetManager()->addEntity(entity);

        entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

        entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

        entity->addComponent<Mesh>("8502_Assets/ancient_wall/ancient_wall.obj");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/ancient_wall/ancient_wall_BaseColor.jpg");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/ancient_wall/ancient_wall_Normal.jpg");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/ancient_wall/ancient_wall_OcclusionRoughnessMetallic.jpg");        
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/ancient_wall/ancient_wall_Roughness.jpg");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/ancient_wall/ancient_wall_AO.jpg");
        entity->getComponent<ShaderProgram>().LoadTextures();
		entity->getComponent<ShaderProgram>().SetFloat(1.455f, "albedo_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(0.790f, "normal_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(0.0f, "metallic_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(0.21f, "roughness_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(0.905f, "occlusion_scalar");
        
        #pragma endregion

        #pragma region gold_columns
		
        // Standard Lit Texture Layout // 0 = albedo // 1 = normal // 2 = metallic // 3 = roughness // 4 = ambient 
        
        entity = new Entity("gold_columns");

        G_GetManager()->addEntity(entity);

        entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

        entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

		entity->addComponent<Mesh>("8502_Assets/gold_columns/gold_columns.obj");
		entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_columns/DirtyGold01_1K_BaseColor.png");
		entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_columns/DirtyGold01_1K_Normal.png");
		entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_columns/DirtyGold01_1K_Metallic.png");
		entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_columns/DirtyGold01_1K_Roughness.png");
		entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/gold_columns/DirtyGold01_1K_AO.png");
		entity->getComponent<ShaderProgram>().LoadTextures();
		entity->getComponent<ShaderProgram>().SetColour(glm::vec4(200.0f / 255.0f, 165.0f / 255.0f, 42.0f / 255.0f, 255.0f / 255.0f), "albedo_color");
		entity->getComponent<ShaderProgram>().SetFloat(0.963f, "sky_box_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(7.552f, "albedo_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(6.11f, "normal_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(1.0f, "metallic_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(0.437f, "roughness_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(3.667f, "occlusion_scalar");

        #pragma endregion

        #pragma region Vases

        #pragma region Vase_Amphora

		// Standard Lit Texture Layout // 0 = albedo // 1 = normal // 2 = metallic // 3 = roughness // 4 = ambient 

        entity = new Entity("Vase_Amphora");
        G_GetManager()->addEntity(entity);

        entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

        entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

        entity->addComponent<Mesh>("8502_Assets/Vases/Vase_Amphora.obj");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Vases/Vase_Amphora_AlbedoTransparency.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Vases/Vase_Amphora_Normal.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("");         
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Vases/Vase_Amphora_MetallicSmoothness.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Vases/");
        entity->getComponent<ShaderProgram>().LoadTextures();
		entity->getComponent<ShaderProgram>().SetFloat(2.1f, "albedo_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(0.3f, "normal_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(0.462f, "metallic_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(0.895, "roughness_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(0.0f, "occlusion_scalar");

        #pragma endregion

        #pragma region Vase_Hydria

		// Standard Lit Texture Layout // 0 = albedo // 1 = normal // 2 = metallic // 3 = roughness // 4 = ambient 

        entity = new Entity("Vase_Hydria");
        G_GetManager()->addEntity(entity);

        entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

        entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

        entity->addComponent<Mesh>("8502_Assets/Vases/Vase_Hydria/Vase_Hydria.obj");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Vases/Vase_Hydria/Vase_Hydria_AlbedoTransparency.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Vases/Vase_Hydria/Vase_Hydria_Normal.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Vases/Vase_Hydria/Vase_Hydria_MetallicSmoothness.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Vases/");
        entity->getComponent<ShaderProgram>().LoadTextures();
		entity->getComponent<ShaderProgram>().SetFloat(2.1f, "albedo_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(0.3f, "normal_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(0.462f, "metallic_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(0.895, "roughness_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(0.0f, "occlusion_scalar");

        #pragma endregion

        #pragma region Vase_Amphora

		// Standard Lit Texture Layout // 0 = albedo // 1 = normal // 2 = metallic // 3 = roughness // 4 = ambient 

        entity = new Entity("Vase_VoluteKrater");
        G_GetManager()->addEntity(entity);

        entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

        entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

        entity->addComponent<Mesh>("8502_Assets/Vases/Vase_VoluteKrater/Vase_VoluteKrater.obj");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Vases/Vase_VoluteKrater/Vase_VoluteKrater_AlbedoTransparency.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Vases/Vase_VoluteKrater/Vase_VoluteKrater_Normal.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Vases/Vase_VoluteKrater/Vase_VoluteKrater_MetallicSmoothness.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/Vases/");
        entity->getComponent<ShaderProgram>().LoadTextures();
		entity->getComponent<ShaderProgram>().SetFloat(2.1f, "albedo_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(0.3f, "normal_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(0.462f, "metallic_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(0.895, "roughness_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(0.0f, "occlusion_scalar");

        #pragma endregion


        #pragma endregion

        #pragma region island_rocks

		// Standard Lit Texture Layout // 0 = albedo // 1 = normal // 2 = metallic // 3 = roughness // 4 = ambient 

        entity = new Entity("island_rocks");
        G_GetManager()->addEntity(entity);

        entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

        entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

        entity->addComponent<Mesh>("8502_Assets/island_rocks/island_rocks.obj");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/island_rocks/T_beech_forest_stones_01_BC_SM.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/island_rocks/T_beech_forest_stones_01_N.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/island_rocks/T_beech_forest_stones_01_DETAIL.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/island_rocks/T_beech_forest_stones_01_MT_AO_SM.png");
        entity->getComponent<ShaderProgram>().LoadTextures();
        entity->getComponent<ShaderProgram>().SetFloat(2.009f, "albedo_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(1.21f, "normal_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(0.0f, "metallic_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(0.59f, "roughness_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(1.381f, "occlusion_scalar");

        #pragma endregion

        #pragma region Marble_Railings

		// Standard Lit Texture Layout // 0 = albedo // 1 = normal // 2 = metallic // 3 = roughness // 4 = ambient 

        entity = new Entity("Marble_Railings");
        G_GetManager()->addEntity(entity);

        entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

        entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

        entity->addComponent<Mesh>("8502_Assets/railing_temp/railing_temp.obj");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_diff_1k.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_nor_1k.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_rough_1k.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_AO_1k.png");
        entity->getComponent<ShaderProgram>().LoadTextures();
		entity->getComponent<ShaderProgram>().SetFloat(1.455f, "albedo_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(0.3f, "normal_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(0.0f, "metallic_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(0.133f, "roughness_scalar");
		entity->getComponent<ShaderProgram>().SetFloat(0.0f, "occlusion_scalar");

        #pragma endregion

        #pragma region Marble_Ground

        // Standard Lit Texture Layout // 0 = albedo // 1 = normal // 2 = metallic // 3 = roughness // 4 = ambient 

        entity = new Entity("marble ground");
        G_GetManager()->addEntity(entity);

        entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");
        
        entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

        entity->addComponent<Mesh>("8502_Assets/marble_ground/Marble_Ground.obj");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_diff_1k.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_nor_1k.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_rough_1k.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_AO_1k.png");
        entity->getComponent<ShaderProgram>().LoadTextures();
		entity->getComponent<ShaderProgram>().SetFloat(1.455f, "albedo_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(0.3f, "normal_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(0.0f, "metallic_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(0.133f, "roughness_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(0.0f, "occlusion_scalar");
    
        #pragma endregion

        #pragma region Statue
        
		// Standard Lit Texture Layout // 0 = albedo // 1 = normal // 2 = metallic // 3 = roughness // 4 = ambient 

        entity = new Entity("Afrodta Statue");
        G_GetManager()->addEntity(entity);

        entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

        entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");
        
        entity->addComponent<Mesh>("8502_Assets/afrodta_statues/afrodta_Statues.obj");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/afrodta_statues/afrodta_Statues_albedo1k.jpg");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/afrodta_statues/afrodta_Statues_normal1k.jpg");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/afrodta_statues/Painted_metal_02_1K_Metallic.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/afrodta_statues/Painted_metal_02_1K_Roughness.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/afrodta_statues/Painted_metal_02_1K_AO.png");
        entity->getComponent<ShaderProgram>().LoadTextures();

		entity->getComponent<ShaderProgram>().SetFloat(1.0f, "albedo_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(0.864f, "normal_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(0.087f, "metallic_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(0.322f, "roughness_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(4.6f, "occlusion_scalar");

        #pragma endregion
        
        #pragma region Tree_box
     
		// Standard Lit Texture Layout // 0 = albedo // 1 = normal // 2 = metallic // 3 = roughness // 4 = ambient 

        entity = new Entity("Tree_Box");
        G_GetManager()->addEntity(entity);

        entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

        entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

        entity->addComponent<Mesh>("8502_Assets/tree_box/tree_box.obj");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_diff_1k.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_nor_1k.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_rough_1k.png");
        entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/marble_ground/marble_01_AO_1k.png");
        entity->getComponent<ShaderProgram>().LoadTextures();
        entity->getComponent<ShaderProgram>().SetFloat(1.0f, "normal_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(1.0f, "metallic_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(1.0f, "roughness_scalar");
        entity->getComponent<ShaderProgram>().SetFloat(1.0f, "occlusion_scalar");
        
        #pragma endregion

        #pragma region Tree_0

	    entity = new Entity("tree_0");
	    G_GetManager()->addEntity(entity);

	    entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

	    entity->getComponent<Transform>().position = glm::vec3(0, -140, 0);

	    entity->addComponent<Mesh>("8502_Assets/tree/tree.obj");
	    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/tree/T_beech_atlas_BC.png");
	    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/tree/T_beech_atlas_N.png");
	    entity->getComponent<ShaderProgram>().AddTexturePath("");
	    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/tree/T_beech_atlas_N.png");
	    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/tree/");
	    entity->getComponent<ShaderProgram>().LoadTextures();
	    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "normal_scalar");
	    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "metallic_scalar");
	    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "roughness_scalar");
	    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "occlusion_scalar");
	    entity->getComponent<ShaderProgram>().SetFloat(0.081f, "alpha_cutoff_amount");
	    entity->getComponent<ShaderProgram>().SetInt(1, "alpha_cutoff_enabled");
        
        #pragma endregion

        #pragma region Tree_1

	    entity = new Entity("tree_1");
	    G_GetManager()->addEntity(entity);

	    entity->addComponent<ShaderProgram>("Shaders/Standard_Lit/Standard_Lit.vert", "Shaders/Standard_Lit/Standard_Lit.frag", "Uniform_Standard_Lit");

	    entity->getComponent<Transform>().position = glm::vec3(69.1f, -140.0f, 77.3f);
	    entity->getComponent<Transform>().scale = glm::vec3(-1, 1, -1);

	    entity->addComponent<Mesh>("8502_Assets/tree/tree.obj");
	    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/tree/T_beech_atlas_BC.png");
	    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/tree/T_beech_atlas_N.png");
	    entity->getComponent<ShaderProgram>().AddTexturePath("");
	    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/tree/T_beech_atlas_N.png");
	    entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/tree/");
	    entity->getComponent<ShaderProgram>().LoadTextures();
	    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "normal_scalar");
	    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "metallic_scalar");
	    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "roughness_scalar");
	    entity->getComponent<ShaderProgram>().SetFloat(1.0f, "occlusion_scalar");
	    entity->getComponent<ShaderProgram>().SetFloat(0.081f, "alpha_cutoff_amount");
	    entity->getComponent<ShaderProgram>().SetInt(1, "alpha_cutoff_enabled");
        
        #pragma endregion

        #pragma region Water

        // 0 = albedo
        // 1 = normal
        // 2 = metallic
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
        entity->getComponent<ShaderProgram>().SetTextureScale(glm::vec2(110,90));
        e_water = entity;

        #pragma endregion

        #pragma region point_Light (4 lights)

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

		entity = new Entity("Point Light1");
        G_GetManager()->addEntity(entity);
		entity->getComponent<Transform>().position = glm::vec3(-20, 15, -15);
        entity->addComponent<LightObject>();
        entity->getComponent<LightObject>().lightType = LightObject::Point;
        entity->getComponent<LightObject>().far_plane = 200;
        entity->getComponent<LightObject>().near_plane = 43;
        entity->addComponent<script_simplebehaviours>();

		entity = new Entity("Point Light2");
		G_GetManager()->addEntity(entity);
		entity->getComponent<Transform>().position = glm::vec3(10, 16, 9);
		entity->addComponent<LightObject>();
		entity->getComponent<LightObject>().lightType = LightObject::Point;
		entity->getComponent<LightObject>().far_plane = 200;
		entity->getComponent<LightObject>().near_plane = 43;
		entity->addComponent<script_simplebehaviours>();

		entity = new Entity("Point Light3");
		G_GetManager()->addEntity(entity);
		entity->getComponent<Transform>().position = glm::vec3(10, 15, -9);
		entity->addComponent<LightObject>();
		entity->getComponent<LightObject>().lightType = LightObject::Point;
		entity->getComponent<LightObject>().far_plane = 200;
		entity->getComponent<LightObject>().near_plane = 43;
		entity->addComponent<script_simplebehaviours>();

		

        std::vector<Entity*> lights = manager->FindLights();


    #pragma endregion

        #pragma region post_processing

		entity = new Entity("post_processing_object");
		G_GetManager()->addEntity(entity);
		entity->addComponent<ShaderProgram>("Shaders/PostProcessing/post_processing.vert", "Shaders/PostProcessing/post_processing.frag", "Uniform_Post_Processing");
		entity->getComponent<ShaderProgram>().AddTexturePath("8502_Assets/water/dudv_halisavkis.png"); // has strange artifact but is seamless
		entity->getComponent<ShaderProgram>().LoadTextures();
        entity->addComponent<Mesh>();
		entity->getComponent<Mesh>().Generate_primitive_Plane();
        entity->getComponent<ShaderProgram>().SetFloat(10.0f, "blurStrength_0");
        entity->getComponent<ShaderProgram>().SetFloat(2.147f, "blurStrength_1");
        entity->getComponent<ShaderProgram>().SetFloat(6.712f, "blurStrength_2");
		post_processing = entity;
        post_processing->isActive(false);

#pragma endregion

        #pragma region Frame Buffer (imGUI Image)

        DepthBuffer d(1024 * 4, 1024 * 4);
        depthBuffers.emplace_back(d);
        
        fbo.GenerateFrameBuffer(editor->lastFrameWindowSize.x, editor->lastFrameWindowSize.y);
        fbo_render_pass.GenerateFrameBuffer(editor->lastFrameWindowSize.x, editor->lastFrameWindowSize.y);
        fbo_post_process.GenerateFrameBuffer(editor->lastFrameWindowSize.x, editor->lastFrameWindowSize.y);
        fbo_post_process2.GenerateFrameBuffer(editor->lastFrameWindowSize.x, editor->lastFrameWindowSize.y);

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

	void test_shadowMap::OnRender() {

        #pragma region UPDATE_FRAME_BUFFERS

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

        #pragma endregion
        
        #pragma region SHADOW_DEPTH_PASS

        for (auto& buffer : depthBuffers) {
            buffer.Bind();
            G_GetManager()->OnRender();
            G_SetShadowMap(buffer.GetDepthBuffer()); // this needs to become list/array
            depthTexturesThisFrame.emplace_back(buffer.GetDepthBuffer());
            buffer.UnBind();
        }

        #pragma endregion

        #pragma region WATER_DEPTH_PASS

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

        #pragma endregion
        
        #pragma region MAIN_RENDER_PASS

		glEnable(GL_DEPTH_TEST);  // We still want depth test
		glDepthFunc(GL_LEQUAL);   // EQUAL should work, too. (Only draw pixels if they are the closest ones)

        fbo.Bind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        G_GetManager()->OnRender();
        
        fbo.UnBind();

        #pragma endregion

        #pragma region POST_PROCESS

        #pragma region POST_PROCESS_DEPTH_OF_FIELD



        // POST PROCESSING
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

        #pragma endregion

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

        #pragma region POST_PROCESS_FLIP_Y_TEXTURE

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

        #pragma endregion
        
        #pragma endregion

    }

    void test_shadowMap::OnExit() {
        G_GetManager()->OnExit();
    }

    void test_shadowMap::OnImGuiRender() {

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

            for (auto& t : e_animated_mesh->getComponent<Anim_Mesh>().textures) {
                if (ImGui::CollapsingHeader("TEXTURE CHECK", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
                    int adjustedWidth = ImGui::GetContentRegionAvailWidth() * 1024 / 1024;
                    ImGui::Image((void*)t->GetTexture(), ImVec2(ImGui::GetContentRegionAvailWidth(), 512));
                }
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
}

