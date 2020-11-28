#include <iostream>
#include "test_ECS.h"
#include "../LightObject.h"
#include "../Mesh.h"
#include "../ShaderProgram.h"
#include "../Camera.h"
#include "../Components/ObjectData.h"



extern uint32_t GetAmountOfEntities();

	EntityManager* GetManager() {
		return g_ecs_manager;
	}

namespace test {



	test_ECS::test_ECS(){

		g_ecs_manager = new EntityManager();

		entity = new Entity("Main Camera");
		entity->addComponent<FPSCamera>();
		entity->getComponent<Transform>().position = glm::vec3(0, 0, -18);
		g_ecs_manager->addEntity(entity);


		for (size_t i = 0; i < 100; i++)
		{
			std::string name = "basic sphere ";
			name.append(std::to_string(i));
			entity = new Entity(name.c_str());
			g_ecs_manager->addEntity(entity);
			entity->addComponent<ShaderProgram>();
			entity->getComponent<ShaderProgram>().SetBaseColor(glm::vec3(glm::ballRand((float)i / 2)));
			if (i % 2){
				entity->addComponent<Mesh>("objectDefaults/cube.obj");
			}
			else if(i % 3){
				entity->addComponent<Mesh>("mesh/pipe.obj");
			}
			else{
				entity->addComponent<Mesh>();
			}
			entity->getComponent<Transform>().position = glm::vec3(glm::ballRand((float)i/2));
			entity->getComponent<Transform>().rotation = glm::vec3(glm::ballRand((float)i/2));
		}

		entity = new Entity();
		g_ecs_manager->addEntity(entity);


		entity = new Entity("POINT LIGHT");
		entity->addComponent<LightObject>();
		g_ecs_manager->addEntity(entity);

	}

	test_ECS::~test_ECS(){

	}

	void test_ECS::OnUpdate(double deltaTime) {
		g_ecs_manager->OnUpdate(deltaTime);
	}

	void test_ECS::OnRender() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		g_ecs_manager->OnRender();
	}

	void test_ECS::OnImGuiRender() {
		
		RenderHierarchy();
		g_ecs_manager->Editor_RenderActiveEditityGui();
	}

	void test_ECS::OnExit(){
		g_ecs_manager->OnExit();
	}

	void test_ECS::RenderHierarchy() {
		{
			//std::cout << GetAmountOfEntities() << std::endl;
			ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
			ImGui::Begin("ECS_Hierarchy");
			{
				for (uint32_t i = 0; i < GetAmountOfEntities(); i++){
					auto label = g_ecs_manager->entities[i]->getComponent<ObjectData>().GetName();
					if (ImGui::Selectable(label, g_ecs_manager->Editor_GetActiveEntity() == i)) {
						g_ecs_manager->Editor_SetActiveEntity(i);
					}
				}
			}

			ImGui::SameLine();
			ImGui::End();
		}
	}

}