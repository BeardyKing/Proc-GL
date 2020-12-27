#ifndef TEST_DOCK_H
#define TEST_DOCK_H

#include "test.h"
#include <iostream>

#include "../Engine/ECS_List.h"
#include "../Engine/EditorGUI.h"

//#include "../FrameBuffer.h"

#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/random.hpp"

namespace test {
	class test_Dock : public Test
	{
	public:
		test_Dock();
		~test_Dock();

		void OnUpdate(double deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnExit()override;

	private:
		//FrameBuffer fbo;

		Entity* entity;
		EditorGUI* editor;
	};
}

#endif // !TEST_DOCK_H
