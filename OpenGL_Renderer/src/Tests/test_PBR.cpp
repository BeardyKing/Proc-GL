#include "test_PBR.h"

namespace test {
	test_PBR::test_PBR() :
		m_amountOfLights(6)
	{
		EntityManager* manager = new EntityManager;
		SetManager(manager);
		{
			//----------------------------------//
			//			  Camera				//
			//----------------------------------//

			entity = new Entity("Main Camera");
			GetManager()->addEntity(entity);

			entity->addComponent<FPSCamera>();
			auto& cam = entity->getComponent<FPSCamera>().usingImGuiWindow = false;
			entity->getComponent<Transform>().position = glm::vec3(0, 0, -18);
			camRef = &entity->getComponent<FPSCamera>();
		}

		{
			//----------------------------------//
			//			  Lights				//
			//----------------------------------//
			for (size_t i = 0; i < m_amountOfLights; i++) {
				std::string name = "basic sphere ";
				GetManager()->addEntity(entity);

				name.append(std::to_string(i));
				entity = new Entity(name.c_str());
				entity->addComponent<LightObject>();
				entity->addComponent<Mesh>();
				entity->addComponent<ShaderProgram>();

				_lightRef.push_back(entity);
			}

			_lightRef[0]->getComponent<Transform>().position = glm::vec3(0.0f, 0.0f, 0.0f) * 0.5f;
			_lightRef[1]->getComponent<Transform>().position = glm::vec3(10.0f, 10.0f, 10.0f) * 0.5f;
			_lightRef[2]->getComponent<Transform>().position = glm::vec3(-13.0f, 0.0f, 10.0f) * 0.5f;
			_lightRef[3]->getComponent<Transform>().position = glm::vec3(7.2f, -5.2f, -13.6f) * 0.5f;
			_lightRef[4]->getComponent<Transform>().position = glm::vec3(-13.0f, 10.0f, 10.0f) * 0.5f;
			_lightRef[5]->getComponent<Transform>().position = glm::vec3(-13.0f, 10.0f, -10.0f) * 0.5f;

			for (auto& light : _lightRef) {
				light->getComponent<ShaderProgram>().SetBaseColor(glm::vec3(1));
			}
		}

		{
			//----------------------------------//
			//			  PBR CUBE				//
			//----------------------------------//

			entity = new Entity("PBR cube");
			GetManager()->addEntity(entity);
			m_pbr_cube = entity;

			entity->getComponent<Transform>().scale = glm::vec3(3);
			entity->addComponent<Mesh>("cube.obj");
			entity->addComponent<ShaderProgram>("pbr.vert", "pbr.frag", "Uniform_PBR");
			entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Base_Color.png");
			entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Normal.png");
			entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Metallic.png");
			entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_Roughness.png");
			entity->getComponent<ShaderProgram>().AddTexturePath("paint/Painted_metal_02_1K_AO.png");
			m_pbr_cube->getComponent<ShaderProgram>().LoadTextures();
		}

		{
			std::string name = "dood ";
			entity = new Entity(name.c_str());
			entity->getComponent<Transform>().scale = glm::vec3(3);
			m_pbr_cube = entity;
			GetManager()->addEntity(entity);
		}

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

		_lightRef[0]->getComponent<Transform>().position.x = 1.5f + 10 * sinf(glm::radians(m_movingLightAngle));
		_lightRef[0]->getComponent<Transform>().position.z = 1.5f + 10 * cosf(glm::radians(m_movingLightAngle));
		_lightRef[0]->getComponent<Transform>().position.y = 3 + (0.5f * sinf(glm::radians(m_movingLightAngle) * 4));

		GetManager()->OnUpdate(deltaTime);
	}

	void test_PBR::OnRender() {
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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


