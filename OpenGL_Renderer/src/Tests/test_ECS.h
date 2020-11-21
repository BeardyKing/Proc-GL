#ifndef TEST_ECS_H
#define TEST_ECS_H
#include "test.h"

#include <iostream>
#include "../ECS/ECS_Manager.h"

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
	};

}

#endif // !TEST_ECS_H
