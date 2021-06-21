#ifndef TEST_ECS_H
#define TEST_ECS_H
#include "test.h"

#include "../Engine/Engine_Core.h"
#include "../Math/Engine_Math.h"


#include <iostream>


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
		void OnExit()override;
	
	private:
		Entity* entity;
		EditorGUI* editor;
	};
}
#endif // !TEST_ECS_H
