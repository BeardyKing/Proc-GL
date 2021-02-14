#ifndef TEST_SHADOWMAP_H
#define TEST_SHADOWMAP_H

#include "test.h"
#include <iostream>

#include "../Engine/ECS_List.h"
#include "../Engine/EditorGUI.h"
#include "../FrameBuffer.h"

namespace test {
	class test_shadowMap : public Test
	{
	public:
		test_shadowMap();
		~test_shadowMap();

		void OnUpdate(double deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnExit()override;

	private:
		FrameBuffer fbo;

		Entity* entity;
		EditorGUI* editor;
		FPSCamera* camera;
		ShaderProgram* debugShader;

	};
}

#endif // !TEST_SHADOWMAP_H
