#include "test_ECS.h"

namespace test {

	test_ECS::test_ECS(){

		Entity* entity = new Entity();
		entity->addComponent<Transform>(glm::vec3(420), glm::vec3(0), glm::vec3(1));

		std::cout << entity->getComponent<Transform>().position.x << std::endl;
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