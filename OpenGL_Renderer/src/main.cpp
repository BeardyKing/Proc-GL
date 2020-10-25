#include <iostream>
#include <sstream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

#include "imGuiStyleSheet.h"

#include "LightObject.h"

#include "ShaderProgram.h"
#include "Texture2D.h"
#include "Camera.h"
#include "Mesh.h"

const char* APP_TITLE = "OpenGL";
int gWindowWidth = 1024;
int gWindowHeight = 768;

GLFWwindow* gWindow = NULL;

bool gFullscreen = false;
bool glWireframe = false;
bool gFlashlightOn = true;
bool gCursorEnabled = true;


FPSCamera fpsCamera(glm::vec3(0.0f, 0.0f, 18.0f));
const double ZOOM_SENSITIVITY = -0.02f;
const float  MOVE_SPEED = 5.0f;
const float  MOUSE_SENSITIVITY = 0.1f;

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

	imGuiStyleSheet style;

	//----------------------------------//
	//				PBR					//
	//----------------------------------//

	Mesh PBR_sphereMesh;
	PBR_sphereMesh.LoadOBJ("sphere.obj");

	ShaderProgram PBR_sphereShader;
	PBR_sphereShader.loadShaders("pbr.vert", "pbr.frag");

	PBR_sphereShader.use();
	PBR_sphereShader.setUniform("albedo", glm::vec3(0.5f, 0.0f, 0.0f));
	PBR_sphereShader.setUniform("ao", 1.0f);

	Texture2D PBR_sphereTexture[5];
	PBR_sphereTexture[0].loadTexture("paint/Painted_metal_02_1K_Base_Color.png",	true);	//albedo
	PBR_sphereTexture[1].loadTexture("paint/Painted_metal_02_1K_Normal.png",		true);	//normal
	PBR_sphereTexture[2].loadTexture("paint/Painted_metal_02_1K_Metallic.png",		true);	//metalic
	PBR_sphereTexture[3].loadTexture("paint/Painted_metal_02_1K_Roughness.png",		true);	//roughness
	PBR_sphereTexture[4].loadTexture("paint/Painted_metal_02_1K_AO.png",			true);	//ambient occlusion 

	//----------------------------------//
	//			POINT LIGHTS			//
	//----------------------------------//

	LightObject pointLights[6];
	pointLights[0].position = glm::vec3(0.0f, 0.0f, 0.0f);
	pointLights[1].position = glm::vec3(10.0f, 10.0f, 10.0f);
	pointLights[2].position = glm::vec3(-13.0f, 0.0f, 10.0f);
	pointLights[3].position = glm::vec3(10.0f, -10.0f, 10.0f);
	pointLights[4].position = glm::vec3(-13.0f, 10.0f, 10.0f);
	pointLights[5].position = glm::vec3(-13.0f, 10.0f, -10.0f);

	//----------------------------------//
	float angle = 10.0f;
	//----------------------------------//


	double lastTime = glfwGetTime();

	//----------------------------------//
	//				LOOP				//
	//----------------------------------//

	



	while (!glfwWindowShouldClose(gWindow)){
		showFPS(gWindow);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime; 

		glfwPollEvents();
		Update(deltaTime);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//----------------------------------//
		//				MVP					//
		//----------------------------------//
		glm::mat4 model, view, projection;	
		view = fpsCamera.GetViewMatrix();
		projection = glm::perspective(fpsCamera.getFOV(), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 100.0f);
		//----------------------------------//
		glm::vec3 viewPos = fpsCamera.GetPosition();
		//----------------------------------//

		//----------------------------------//
		//			PBR MATERIAL			//
		//----------------------------------//

		PBR_sphereShader.use();

		glm::vec3 pbr_Pos(0.0f, 0.0f, 0.0f);
		glm::vec3 pbr_Scale(5.0f, 5.0f, 5.0f);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f)) * glm::scale(model, pbr_Scale);

		PBR_sphereShader.setUniform("model", model);
		PBR_sphereShader.setUniform("view", view);
		PBR_sphereShader.setUniform("projection", projection);
		PBR_sphereShader.setUniform("camPos", viewPos);

		PBR_sphereTexture[0].bind(0);
		PBR_sphereTexture[1].bind(1);
		PBR_sphereTexture[2].bind(2);
		PBR_sphereTexture[3].bind(3);
		PBR_sphereTexture[4].bind(4);

		PBR_sphereShader.setUniformSampler("albedoMap",		0);
		PBR_sphereShader.setUniformSampler("normalMap",		1);
		PBR_sphereShader.setUniformSampler("metallicMap",	2);
		PBR_sphereShader.setUniformSampler("roughnessMap",	3);
		PBR_sphereShader.setUniformSampler("aoMap",			4);

		PBR_sphereMesh.Draw();
		
		//----------------------------------//
		//		PBR SET LIGHT UNIFORMS		//
		//----------------------------------//

		int amountOfLights = sizeof(pointLights) / sizeof(pointLights[0]);

		PBR_sphereShader.use();
		PBR_sphereShader.setUniform("amountOfLights", amountOfLights);

		for (unsigned int i = 0; i < amountOfLights; ++i) {
			glm::vec3 newPos = pointLights[i].position + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
			newPos = pointLights[i].position;

			std::string str1 = "lightPositions[" + std::to_string(i) + "]";
			const char* c1 = str1.c_str();
			PBR_sphereShader.setUniform(c1, newPos);

			std::string str2 = "lightColors[" + std::to_string(i) + "]";
			const char* c2 = str2.c_str();
			PBR_sphereShader.setUniform(c2, pointLights[i].color);
		}

		//----------------------------------//
		//			Moving Light			//
		//----------------------------------//

		angle += (float)deltaTime * 90.0f;

		pointLights[0].position.x = 1.5f + 10 * sinf(glm::radians(angle));
		pointLights[0].position.z = 1.5f + 10 * cosf(glm::radians(angle));
		pointLights[0].position.y = 3 + (0.5f * sinf(glm::radians(angle) * 4));

		//----------------------------------//
		//			Render Lights			//
		//----------------------------------//

		for (unsigned int i = 0; i < amountOfLights; i++) {
			model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(0.5f));
			model = glm::translate(model, pointLights[i].position);

			pointLights[i].m_Shader->use();
			pointLights[i].m_Shader->setUniform("lightCol", pointLights[i].color);
			pointLights[i].m_Shader->setUniform("model", model);
			pointLights[i].m_Shader->setUniform("view", view);
			pointLights[i].m_Shader->setUniform("projection", projection);
			pointLights[i].m_Mesh->Draw();
		}

		//----------------------------------//
		//				IMGUI				//
		//----------------------------------//

		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			{
				ImGui::Begin("Hello GUI");       

				ImGui::Text("Number of lights in scene : %d", amountOfLights);
				ImGui::End();
			}
			// Rendering
			ImGui::Render();
			int display_w, display_h;
			glfwGetFramebufferSize(gWindow, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}


		//----------------------------------//
		//				END					//
		//----------------------------------//


		glfwSwapBuffers(gWindow);
		lastTime = currentTime;
	}

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

	//hides and grabs cursor, unlimited movement
	if (!gCursorEnabled){
		glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);
	}

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

	//----------------------------------//
	//				IMGUI				//
	//----------------------------------//

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(gWindow, true);
	ImGui_ImplOpenGL3_Init(glsl_version);


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

	if (key == GLFW_KEY_F && action == GLFW_PRESS){
		gFlashlightOn = !gFlashlightOn;
	}

	if (glfwGetKey(gWindow, GLFW_KEY_E) == GLFW_PRESS) {
		gCursorEnabled = !gCursorEnabled;
		if (gCursorEnabled) {
			glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else {
			glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}


}

void glfw_OnFrameBufferSize(GLFWwindow* window, int width, int height) {
	gWindowWidth	= width;
	gWindowHeight	= height;

	glViewport(0, 0, gWindowWidth, gWindowHeight);
}

void glfw_OnMouseMove(GLFWwindow* window, double posX, double posY) {
	/*static glm::vec2 lastMousePos = glm::vec2(0, 0);

	if (glfwGetMouseButton(gWindow, GLFW_MOUSE_BUTTON_LEFT) == 1){
		gYaw -= ((float)posX - lastMousePos.x) * MOUSE_SENSITIVITY;
		gPitch += ((float)posY - lastMousePos.y) * MOUSE_SENSITIVITY;
	}

	if (glfwGetMouseButton(gWindow, GLFW_MOUSE_BUTTON_RIGHT) == 1){
		float dx = 0.01f * ((float)posX - lastMousePos.x);
		float dy = 0.01f * ((float)posY - lastMousePos.y);
		gRadius += dx - dy;
	}

	lastMousePos.x = (float)posX;
	lastMousePos.y = (float)posY;*/
}

void glfw_OnMouseScroll(GLFWwindow* window, double deltaZ, double deltaY) {
	double fov = fpsCamera.getFOV() + deltaY * ZOOM_SENSITIVITY;

	fov = glm::clamp(fov, 1.0, 120.0);

	fpsCamera.setFOV((float)fov);
}


void Update(double elapsedTime) {

	
	
	if(!gCursorEnabled){
		double mouseX, mouseY;


		glfwGetCursorPos(gWindow, &mouseX, &mouseY);
		fpsCamera.Rotate((float)(gWindowWidth / 2.0 - mouseX) * MOUSE_SENSITIVITY, (float)(gWindowHeight / 2.0 - mouseY) * MOUSE_SENSITIVITY);

		glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);
		//sprint
		float moveSpeedDelta = 1;
		if (glfwGetKey(gWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
			moveSpeedDelta = 4;
		}


		//forwards / backwards
		if (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS){
			fpsCamera.Move((MOVE_SPEED * moveSpeedDelta) * (float)elapsedTime * fpsCamera.GetLook());
		}
		else if (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS) {
			fpsCamera.Move((MOVE_SPEED * moveSpeedDelta) * (float)elapsedTime * -fpsCamera.GetLook());
		}

		// left / right
		if (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS) {
			fpsCamera.Move((MOVE_SPEED * moveSpeedDelta) * (float)elapsedTime * -fpsCamera.GetRight());
		}
		else if (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS) {
			fpsCamera.Move((MOVE_SPEED * moveSpeedDelta) * (float)elapsedTime * fpsCamera.GetRight());
		}

		//up / down
		if (glfwGetKey(gWindow, GLFW_KEY_Z) == GLFW_PRESS) {
			fpsCamera.Move((MOVE_SPEED * moveSpeedDelta) * (float)elapsedTime * fpsCamera.GetUp());
		}
		else if (glfwGetKey(gWindow, GLFW_KEY_X) == GLFW_PRESS) {
			fpsCamera.Move((MOVE_SPEED * moveSpeedDelta) * (float)elapsedTime * -fpsCamera.GetUp());
		}
	}

}

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



