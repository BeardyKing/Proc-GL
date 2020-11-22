#ifndef TEST_ECS_H
#define TEST_ECS_H
#include "test.h"

#include <iostream>
#include "../Components/Transform.h"
#include "../Components/ObjectData.h"
#include "../ECS/Entity.h"
#include "../ECS/EntityManager.h"

namespace test {
	class test_ECS : public Test
	{
	public:
		test_ECS();
		~test_ECS();

	private:
		void OnUpdate(double deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	
	private:
		EntityManager* manager;
		Entity* entity;
	};

}

#endif // !TEST_ECS_H
