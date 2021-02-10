#include "test_PBR.h"

namespace test {
	test_PBR::test_PBR() :
		m_amountOfLights(6)
	{
		EntityManager* manager = new EntityManager;
		SetManager(manager);

		//----------------------------------//
		//			  Camera				//
		//----------------------------------//

		entity = new Entity("Main Camera");
		entity->addComponent<FPSCamera>();
		auto& cam = entity->getComponent<FPSCamera>().usingImGuiWindow = false;
		entity->getComponent<Transform>().position = glm::vec3(0, 0, -18);
		camRef = &entity->getComponent<FPSCamera>();
		GetManager()->addEntity(entity);

		//----------------------------------//
		//			  Lights				//
		//----------------------------------//

		for (size_t i = 0; i < m_amountOfLights; i++)
		{
			std::string name = "basic sphere ";
			name.append(std::to_string(i));
			entity = new Entity(name.c_str());
			entity->addComponent<LightObject>();
			entity->getComponent<LightObject>().lightType;
			entity->addComponent<Mesh>();
			entity->addComponent<ShaderProgram>();
			GetManager()->addEntity(entity);

			m_point_lights.push_back(entity);
		}

		m_point_lights[0]->getComponent<Transform>().position = glm::vec3(0.0f, 0.0f, 0.0f)			* 0.5f;
		m_point_lights[1]->getComponent<Transform>().position = glm::vec3(10.0f, 10.0f, 10.0f)		* 0.5f;
		m_point_lights[2]->getComponent<Transform>().position = glm::vec3(-13.0f, 0.0f, 10.0f)		* 0.5f;
		m_point_lights[3]->getComponent<Transform>().position = glm::vec3(7.2f, -5.2f, -13.6f)		* 0.5f;
		m_point_lights[4]->getComponent<Transform>().position = glm::vec3(-13.0f, 10.0f, 10.0f)		* 0.5f;
		m_point_lights[5]->getComponent<Transform>().position = glm::vec3(-13.0f, 10.0f, -10.0f)	* 0.5f;

		//----------------------------------//
		//			  PBR CUBE				//
		//----------------------------------//

		std::string name = "PBR cube ";
		entity = new Entity(name.c_str());
		entity->getComponent<Transform>().scale = glm::vec3(3);
		m_pbr_cube = entity;
		GetManager()->addEntity(entity);

		m_PBR_sphereMesh = std::make_unique<Mesh>();
		m_PBR_sphereMesh->LoadOBJ("cube.obj");

		m_PBR_sphereShader = std::make_unique<ShaderProgram>();
		m_PBR_sphereShader->loadShaders("pbr.vert", "pbr.frag");

		m_PBR_sphereShader->use();
		m_PBR_sphereShader->setUniform("albedo", glm::vec3(0.5f, 0.0f, 0.0f));
		m_PBR_sphereShader->setUniform("ao", 1.0f);

		m_PBR_sphereTexture = std::make_unique<Texture2D[]>(5);
		m_PBR_sphereTexture[0].loadTexture("paint/Painted_metal_02_1K_Base_Color.png", true);	//albedo
		m_PBR_sphereTexture[1].loadTexture("paint/Painted_metal_02_1K_Normal.png", true);		//normal
		m_PBR_sphereTexture[2].loadTexture("paint/Painted_metal_02_1K_Metallic.png", true);		//metalic
		m_PBR_sphereTexture[3].loadTexture("paint/Painted_metal_02_1K_Roughness.png", true);	//roughness
		m_PBR_sphereTexture[4].loadTexture("paint/Painted_metal_02_1K_AO.png", true);			//ambient occlusion 

	}

	test_PBR::~test_PBR() {
		m_PBR_sphereMesh.reset();
		m_PBR_sphereShader.reset();
		m_PBR_sphereTexture.reset();
	}

	void test_PBR::OnUpdate(double deltaTime){

		//----------------------------------//
		//			Moving Light			//
		//----------------------------------//

		m_movingLightAngle += (float)deltaTime * 90.0f;	// rotate lights

		m_point_lights[0]->getComponent<Transform>().position.x = 1.5f + 10 * sinf(glm::radians(m_movingLightAngle));
		m_point_lights[0]->getComponent<Transform>().position.z = 1.5f + 10 * cosf(glm::radians(m_movingLightAngle));
		m_point_lights[0]->getComponent<Transform>().position.y = 3 + (0.5f * sinf(glm::radians(m_movingLightAngle) * 4));

		GetManager()->OnUpdate(deltaTime);
	}

	void test_PBR::OnRender() {
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//----------------------------------//
		//				MVP					//
		//----------------------------------//

		glm::mat4 model, view, projection;
		view = camRef->GetViewMatrix();
		projection = glm::perspective(camRef->getFOV(), (float)G_GetWindowWidth() / (float)G_GetWindowHeight(), 0.1f, 100.0f);
		
		//----------------------------------//
		//			PBR MATERIAL			//
		//----------------------------------//

		m_PBR_sphereShader->use();

		model = m_pbr_cube->getComponent<Transform>().GetTransformMatrix();

		m_PBR_sphereShader->setUniform("model", model);
		m_PBR_sphereShader->setUniform("view", view);
		m_PBR_sphereShader->setUniform("projection", projection);
		m_PBR_sphereShader->setUniform("camPos", camRef->GetPosition());

		m_PBR_sphereTexture[0].bind(0);
		m_PBR_sphereTexture[1].bind(1);
		m_PBR_sphereTexture[2].bind(2);
		m_PBR_sphereTexture[3].bind(3);
		m_PBR_sphereTexture[4].bind(4);

		m_PBR_sphereShader->setUniformSampler("albedoMap",		0);
		m_PBR_sphereShader->setUniformSampler("normalMap",		1);
		m_PBR_sphereShader->setUniformSampler("metallicMap",	2);
		m_PBR_sphereShader->setUniformSampler("roughnessMap",	3);
		m_PBR_sphereShader->setUniformSampler("aoMap",			4);
						
		m_PBR_sphereMesh->Draw();

		//----------------------------------//
		//		PBR SET LIGHT UNIFORMS		//
		//----------------------------------//		

		m_PBR_sphereShader->use();
		m_PBR_sphereShader->setUniform("amountOfLights", m_amountOfLights);

		for (unsigned int i = 0; i < m_amountOfLights; ++i) {
			std::string str1 = "lightPositions[" + std::to_string(i) + "]";
			m_PBR_sphereShader->setUniform(str1.c_str(), m_point_lights[i]->getComponent<Transform>().position);	// set shader uniform for lightPosition[i]

			std::string str2 = "lightColors[" + std::to_string(i) + "]";
			m_PBR_sphereShader->setUniform(str2.c_str(), m_point_lights[i]->getComponent<LightObject>().color);		// set shader uniform for lightColors[i]
		}

		GetManager()->OnRender();
	}

	void test_PBR::OnImGuiRender() {
		RenderHierarchy();
		GetManager()->Editor_RenderActiveInspector();
	}

	void test_PBR::OnExit() {
		GetManager()->OnExit();
	}

	void test_PBR::RenderHierarchy() {
		{
			ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
			ImGui::Begin("ECS_Hierarchy");
			{
				for (uint32_t i = 0; i < GetAmountOfEntities(); i++) {
					auto label = GetManager()->entities[i]->getComponent<ObjectData>().GetName();
					if (ImGui::Selectable(label, GetManager()->Editor_GetActiveEntity() == i)) {
						GetManager()->Editor_SetActiveEntity(i);
					}
				}
			}

			ImGui::SameLine();
			ImGui::End();
		}
	}

}


