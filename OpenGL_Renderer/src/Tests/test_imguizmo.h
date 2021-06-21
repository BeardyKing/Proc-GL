#ifndef TEST_IMGUIZMO
#define TEST_IMGUIZMO

#include "test.h"

#include <GL/glew.h>
#include "../Engine/Engine_Core.h"
#include "../Math/Engine_Math.h"


#include <iostream>
#include <sstream>
#include <vector>


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
