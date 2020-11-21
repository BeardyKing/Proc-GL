#include "test_ECS.h"

namespace test {

	test_ECS::test_ECS(){
		std::cout << "INIT TEST ECS" << std::endl;
		ECS_Manager ecs;
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