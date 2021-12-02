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
		Entity* create_camera();
		Entity* create_sky_box();
		Entity* create_water();
		Entity* create_point_light_shadow();
		Entity* create_point_light();
		Entity* create_post_processing();
		Entity* create_random_default_object(int i);

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

		Entity* e_skybox;
		Entity* e_water;
		std::vector<GLuint> matTextures;

		bool show_editor = true;

	};
}

#endif // !TEST_POST_PROCESSING_H
