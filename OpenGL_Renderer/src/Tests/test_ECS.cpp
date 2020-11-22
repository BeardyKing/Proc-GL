#include "test_ECS.h"

namespace test {

	test_ECS::test_ECS(){

		manager = new EntityManager();
		entity = new Entity();

		manager->addEntity(entity);
		
		entity->getComponent<Transform>().position.x = 420;
		std::cout << entity->getComponent<Transform>().position.x << std::endl;
		std::cout << entity->hasComponent<Transform>() << std::endl;

		entity = new Entity();
		manager->addEntity(entity);
		
		entity = new Entity();
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
		manager->OnImGuiRender();
	}

}