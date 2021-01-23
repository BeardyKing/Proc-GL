#ifndef TEST_IMGUIZMO
#define TEST_IMGUIZMO

#include "test.h"
#include <iostream>

#include "../Engine/ECS_List.h"
#include "../Engine/EditorGUI.h"

#include "../FrameBuffer.h"

#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/random.hpp"

namespace test {
	class test_imguizmo : public Test
	{
	public:
		test_imguizmo();
		~test_imguizmo();

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

#endif // !TEST_IMGUIZMO
