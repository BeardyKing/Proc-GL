#ifndef TEST_ECS_H
#define TEST_ECS_H
#include "test.h"

#include <iostream>
#include "../Engine/ECS_List.h"
#include "../Engine/EditorGUI.h"

#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/random.hpp"



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
