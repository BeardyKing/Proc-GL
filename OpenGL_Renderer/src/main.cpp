#include <iostream>
#include <sstream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

#include "imGuiStyleSheet.h"

#include "Tests/test.h"
#include "Tests/test_PBR.h";
#include "Tests/test_Dock.h"
#include "Tests/test_ECS.h"

const char* APP_TITLE = "OpenGL - Tests";
static int gWindowWidth = 1024;
static int gWindowHeight = 768;
static bool gWindowResizedFlag = false;

bool G_GetWindowResizeFlag() {
	return gWindowResizedFlag;
}
int G_GetWindowWidth() {
	return gWindowWidth;
}
int G_GetWindowHeight() {
	return gWindowHeight;
}

GLFWwindow* gWindow = NULL;

bool gFullscreen = false;	// to be deprecated
bool glWireframe = false;	// to be deprecated
bool gCursorEnabled = true; // to be deprecated

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_OnFrameBufferSize(GLFWwindow* window, int width, int height);
void glfw_OnMouseMove(GLFWwindow* window, double posX, double posY);
void glfw_OnMouseScroll(GLFWwindow* window, double deltaX, double deltaY);

void Update(double eleapsedTime);

void showFPS(GLFWwindow* window);
bool InitOpenGL();

int main(){
	if (!InitOpenGL()){ 
		std::cout << "INT FAILED" << std::endl;
		return -1;
	}

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	test::Test* currentTest = nullptr;
	test::TestMenu* testMenu = new test::TestMenu(currentTest);
	currentTest = testMenu;
	testMenu->RegisterTest<test::test_PBR>("PBR 5 point light");
	testMenu->RegisterTest<test::test_Dock>("docking test");
	testMenu->RegisterTest<test::test_ECS>("ECS test");
	
	double lastTime = glfwGetTime();

	// Main loop
	while (!glfwWindowShouldClose(gWindow))
	{
		showFPS(gWindow);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime; 

		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (currentTest) {

			currentTest->OnUpdate(deltaTime);
			currentTest->OnRender();
			ImGui::Begin("Test");
			currentTest->OnImGuiRender();

			if (currentTest != testMenu && ImGui::Button("<-")) {
				currentTest->OnExit();
				delete currentTest;
				currentTest = testMenu;
			}

			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers(gWindow);
		lastTime = currentTime;
		gWindowResizedFlag = false;
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(gWindow);
	glfwTerminate();
	return 0;
}

bool InitOpenGL() {
	if (!glfwInit()){
		std::cerr << "GLFW DID NOT INIT" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	const char* glsl_version = "#version 330";

	if (gFullscreen) {
		GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* pVmode = glfwGetVideoMode(pMonitor);
		if (pVmode != NULL) {
			gWindow = glfwCreateWindow(pVmode->width, pVmode->height, APP_TITLE, pMonitor, NULL);
		}
	}
	else {
		gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
	}

	if (gWindow == NULL) {
		std::cerr << "GLFW Window Did not init" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(gWindow);

	// input
	glfwSetKeyCallback(gWindow, glfw_onKey);
	glfwSetCursorPosCallback(gWindow, glfw_OnMouseMove);
	glfwSetScrollCallback(gWindow, glfw_OnMouseScroll);
	glfwSetFramebufferSizeCallback(gWindow, glfw_OnFrameBufferSize);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cerr << "GLEW Did not init" << std::endl;
		glfwTerminate();
		return false;
	}

	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glViewport(0, 0, gWindowWidth, gWindowHeight);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    ////io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(gWindow, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	imGuiStyleSheet style;

	return true;
}


void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode) {
	
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_Q && action == GLFW_PRESS){
		glWireframe = !glWireframe;
		if (glWireframe){
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	if (glfwGetKey(gWindow, GLFW_KEY_E) == GLFW_PRESS) {
		gCursorEnabled = !gCursorEnabled;
		if (gCursorEnabled) {
			glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else {
			glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetCursorPos(gWindow, 0, 0);
		}
	}
}

void glfw_OnFrameBufferSize(GLFWwindow* window, int width, int height) {
	gWindowWidth	= width;
	gWindowHeight	= height;

	gWindowResizedFlag = true;

	glViewport(0, 0, gWindowWidth, gWindowHeight);
}

void glfw_OnMouseMove(GLFWwindow* window, double posX, double posY) {}
void glfw_OnMouseScroll(GLFWwindow* window, double deltaZ, double deltaY) {}
void Update(double elapsedTime) {}

void showFPS(GLFWwindow* window) {
	static double previousSecond = 0.0;
	static int frameCount = 0;
	double elapsedSeconds;
	double currentSeconds = glfwGetTime(); // number of seconds since time since GLFW started

	elapsedSeconds = currentSeconds - previousSecond;

	//limit update 4 times per second
	if (elapsedSeconds > 0.25){
		previousSecond = currentSeconds;
		double fps = (double)frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / fps;

		std::ostringstream outs;
		outs.precision(3);
		outs << std::fixed 
			 << APP_TITLE << "  " 
			 << "FPS: " << fps << "   " 
			 << "Frame Time: " << msPerFrame << " (ms)";
		glfwSetWindowTitle(window, outs.str().c_str());
		frameCount = 0;
	}
	frameCount++;
}



