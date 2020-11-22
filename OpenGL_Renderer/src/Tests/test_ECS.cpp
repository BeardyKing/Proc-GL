#include "test_ECS.h"
#include "../LightObject.h"

extern uint32_t GetAmountOfEntities();

namespace test {

	test_ECS::test_ECS(){

		manager = new EntityManager();
		entity = new Entity("ENTITY BOI");

		manager->addEntity(entity);
		
		entity->getComponent<Transform>().position.x = 420;
		std::cout << entity->getComponent<Transform>().position.x << std::endl;
		std::cout << entity->hasComponent<Transform>() << std::endl;

		entity = new Entity("FRIEND");
		manager->addEntity(entity);
		
		entity = new Entity("BASE ENTITY");
		manager->addEntity(entity);

		entity = new Entity("POINT LIGHT");
		entity->addComponent<LightObject>();
		manager->addEntity(entity);

	}

	test_ECS::~test_ECS(){

	}

	void test_ECS::OnUpdate(double deltaTime) {
		manager->OnUpdate(deltaTime);
	}

	void test_ECS::OnRender() {
		manager->OnRender();
	}

	void test_ECS::OnImGuiRender() {
		
		RenderHierarchy();
		manager->Editor_RenderActiveEditityGui();
	}

	void test_ECS::OnExit(){
		manager->OnExit();
	}

	void test_ECS::RenderHierarchy() {
		{
			std::cout << GetAmountOfEntities() << std::endl;
			ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
			ImGui::Begin("ECS_Hierarchy");
			{
				for (uint32_t i = 0; i < GetAmountOfEntities(); i++){
					auto label = manager->entities[i]->getComponent<ObjectData>().GetName();
					if (ImGui::Selectable(label, manager->Editor_GetActiveEntity() == i)) {
						manager->Editor_SetActiveEntity(i);
					}
				}
			}

			ImGui::SameLine();
			ImGui::End();
		}
	}

}