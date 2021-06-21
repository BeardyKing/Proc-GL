#ifndef TEST_DOCK_H
#define TEST_DOCK_H

#include "test.h"
#include <iostream>

#include <GL/glew.h>

#include "../Engine/Engine_Core.h"
#include "../Math/Engine_Math.h"


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
		FrameBuffer fbo;

		Entity* entity;
		EditorGUI* editor;
	};
}

#endif // !TEST_DOCK_H
