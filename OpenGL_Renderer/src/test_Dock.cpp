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
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
	}
}