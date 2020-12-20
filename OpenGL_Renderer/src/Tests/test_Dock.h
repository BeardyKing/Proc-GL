#ifndef TEST_DOCK_H
#define TEST_DOCK_H

#include "test.h"

#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_impl_glfw.h"
#include "../vendor/imgui/imgui_impl_opengl3.h"

#include "../LightObject.h"
#include "../ShaderProgram.h"
#include "../Texture2D.h"
#include "../Mesh.h"
#include "../Camera.h"
#include "../FrameBuffer.h"

#include <iostream>
#include "../Components/Transform.h"
#include "../Components/ObjectData.h"
#include "../ECS/Entity.h"
#include "../ECS/EntityManager.h"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/random.hpp"
#include "test_ECS.h"
#include "../LightObject.h"
#include "../Mesh.h"
#include "../ShaderProgram.h"
#include "../Camera.h"
#include "../Components/ObjectData.h"

int G_GetWindowWidth();
int G_GetWindowHeight(); 
bool G_GetWindowResizeFlag();


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

		void UpdateFrameBufferTextureSize();
		void RenderHierarchy();

	private:
		
		FrameBuffer fbo;

		ImVec2 lastFrameWindowSize;
		bool windowSizeChangeFlag;

		Texture2D m_Texture2D;
		Texture2D m_frameBufferTexture;

		uint32_t counter;

		Entity* entity;

	};
}

#endif // !TEST_DOCK_H
