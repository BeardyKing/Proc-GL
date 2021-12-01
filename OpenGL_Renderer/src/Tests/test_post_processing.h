#ifndef TEST_POST_PROCESSING_H
#define TEST_POST_PROCESSING_H

#include "test.h"

#include "../Engine/Engine_Core.h"
#include "../Math/Engine_Math.h"

#include <vector>

namespace test {
	class test_post_processing : public Test
	{
	public:
		test_post_processing();
		~test_post_processing() override;

		void OnUpdate(double deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnExit()override;

	private:
		void update_frame_buffer_size();

		void render_texture_shadow_map();
		void render_texture_no_transparent();

		void draw();

		void post_processing_flip();
		void post_processing_depth_of_field();
		void post_processing_vignette();
	private:
		void create_camera();
		void create_sky_box();
		void create_water();
		void create_point_light_shadow();
		void create_point_light();
		void create_post_processing();

		void initialise_frame_buffers();

	private:
		FrameBuffer fbo;
		RenderPass fbo_post_process;
		RenderPass fbo_post_process2;
		RenderPass fbo_render_pass;

		std::vector<DepthBuffer> depthBuffers;
		std::vector<GLuint> depthTexturesThisFrame;

		Entity* entity;
		EditorGUI* editor;
		FPSCamera* camera;

		Entity* post_processing;

		//SKYBOX TEST
		Entity* e_skybox;
		Entity* e_water;
		std::vector<GLuint> matTextures;

		bool show_editor = true;

	};
}

#endif // !TEST_POST_PROCESSING_H
