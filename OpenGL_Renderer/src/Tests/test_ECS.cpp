#include "test_ECS.h"

namespace test {

	test_ECS::test_ECS(){

		Entity* entity = new Entity();
		entity->getComponent<Transform>().position.x = 420;
		std::cout << entity->getComponent<Transform>().position.x << std::endl;
		std::cout << entity->hasComponent<Transform>() << std::endl;
	}

	test_ECS::~test_ECS(){

	}

	void test_ECS::OnUpdate(double deltaTime) {

	}

	void test_ECS::OnRender() {

	}

	void test_ECS::OnImGuiRender() {

	}

}