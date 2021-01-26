#ifndef TEST_DEFAULT_H
#define TEST_DEFAULT_H

#include "../test.h"
#include <iostream>

#include "../../Engine/ECS_List.h"
#include "../../Engine/EditorGUI.h"

#include "../../FrameBuffer.h"

#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/random.hpp"

namespace test {
	class test_Default : public Test
	{
	public:
		test_Default();
		~test_Default();

		void OnUpdate(double deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnExit()override;

	private:
		FrameBuffer fbo;

		Entity* entity;
		EditorGUI* editor;
	};
}

#endif // !TEST_DEFAULT_H
