#ifndef TEST_SHADOWMAP_H
#define TEST_SHADOWMAP_H

#include "test.h"

#include "../Engine/Engine_Core.h"
#include "../Math/Engine_Math.h"
#include "../Engine/Buffers/Depth_Pass.h"

#include <iostream>
#include <sstream>
#include <vector>

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
		RenderPass fbo_post_process;
		RenderPass fbo_render_pass;
		std::vector<DepthBuffer> depthBuffers;
		std::vector<GLuint> depthTexturesThisFrame;

		Entity* entity;
		EditorGUI* editor;
		FPSCamera* camera;
		ShaderProgram* depthShader;

		Entity* post_processing;

		//SKYBOX TEST
		Entity* e_skybox;
		Entity* e_water;
		Entity* e_animated_mesh;
		std::vector<GLuint> matTextures;


	};
}

#endif // !TEST_SHADOWMAP_H
