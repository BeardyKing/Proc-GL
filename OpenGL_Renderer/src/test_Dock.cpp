#include "test_Dock.h"

#include <iostream>
#include <sstream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

namespace test {
	test_Dock::test_Dock(){

	}

	test_Dock::~test_Dock() {}

	void test_Dock::OnUpdate(double deltaTime) {}

	void test_Dock::OnRender() {}

	void test_Dock::OnImGuiRender() {

		ImGui::Begin("Transform");
		ImGui::Begin("Transform2");
		ImGui::End();

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
}