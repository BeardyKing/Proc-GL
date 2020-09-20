#include <iostream>
#include <sstream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

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

//OrbitCamera orbitCamera;
//float gYaw = 0.0f;
//float gPitch = 0.0f;
//float gRadius = 10.0f;
//const float MOUSE_SENSITIVITY = 0.25f;

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

	//model positions
	glm::vec3 modelPos[] = {
		glm::vec3(-5.0f,-15.5f,0.0f),
		glm::vec3(0.0f,-15.0f,0.0f),
		glm::vec3(-2.0f,-15.0f,-3.0f),
		glm::vec3(0.0f,-15.0f,0.0f)
	};

	glm::vec3 modelScale[] = {
		glm::vec3(1.0f,1.0f,1.0f),
		glm::vec3(5.0f,0.2f,5.0f),
		glm::vec3(2.0f,2.0f,2.0f),
		glm::vec3(0.5f,0.5f,0.5f)
	};
	// Load meshes and textures
	const int numModels = 4;
	Mesh mesh[numModels];
	Texture2D texture[numModels];

	//load mesh
	mesh[0].LoadOBJ("mesh/box.obj");
	mesh[1].LoadOBJ("mesh/ground.obj");
	mesh[2].LoadOBJ("mesh/pipe.obj");
	mesh[3].LoadOBJ("frog/frog.obj");

	//load texture
	texture[0].loadTexture("Wall/Brick_wall_02_1K_Base_Color.png",				true);
	texture[1].loadTexture("Ground/Brushed_Metal_Tiles_04_1K_Base_Color.png",	true);
	texture[2].loadTexture("mesh/polygon_texture.png",							true);
	texture[3].loadTexture("frog/frog.png",										true);

	// ---------- --------- ----------
	// ---------- PBR		----------
	Mesh PBR_sphereMesh;
	PBR_sphereMesh.LoadOBJ("sphere.obj");

	ShaderProgram PBR_sphereShader;
	PBR_sphereShader.loadShaders("pbr.vert", "pbr.frag");

	PBR_sphereShader.use();
	PBR_sphereShader.setUniform("albedo", glm::vec3(0.5f, 0.0f, 0.0f));
	PBR_sphereShader.setUniform("ao", 1.0f);

	Texture2D PBR_sphereTexture[5];
	PBR_sphereTexture[0].loadTexture("paint/Painted_metal_02_1K_Base_Color.png",	true);	//albedo
	PBR_sphereTexture[1].loadTexture("paint/Painted_metal_02_1K_Normal.png",		true);		//normal
	PBR_sphereTexture[2].loadTexture("paint/Painted_metal_02_1K_Metallic.png",		true);		//metalic
	PBR_sphereTexture[3].loadTexture("paint/Painted_metal_02_1K_Roughness.png",		true);	//roughness
	PBR_sphereTexture[4].loadTexture("paint/Painted_metal_02_1K_AO.png",			true);			//ambient occlusion 

	// ---------- --------- ----------
	// ---------- CUBE MAP	----------
	Mesh CubeMapMesh;
	CubeMapMesh.LoadOBJ("cube.obj");

	Texture2D cubeMapTexture;
	cubeMapTexture.loadHDRTexture("Mans_Outside_2k.hdr");

	ShaderProgram cubemapShader;
	cubemapShader.loadShaders("cubemap.vert", "cubemap.frag");

	// lights
	// ------
	glm::vec3 lightPositions[] = {
		glm::vec3(-13.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-13.0f, 0.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
	};
	glm::vec3 lightColors[] = {
		glm::vec3(150.0f, 150.0f, 150.0f),
		glm::vec3(150.0f, 150.0f, 150.0f),
		glm::vec3(150.0f, 150.0f, 150.0f),
		glm::vec3(150.0f, 150.0f, 150.0f)
	};
	int nrRows = 1;
	int nrColumns = 1;
	float spacing = 1;


	// ---------- --------- ----------
	// ---------- point_Pos ----------
	Mesh lightMesh;
	lightMesh.LoadOBJ("light.obj");

	ShaderProgram lightbulbShader;
	lightbulbShader.loadShaders("basic.vert", "basic.frag");

	// ---------- ----------- ----------
	// ---------- mesh shader ----------
	ShaderProgram lightingShader;
	lightingShader.loadShaders("lightingMultiple.vert", "lightingMultiple.frag");

	


	float angle = 10.0f;
	double lastTime = glfwGetTime();

	

	while (!glfwWindowShouldClose(gWindow)){
		showFPS(gWindow);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime; 

		glfwPollEvents();
		Update(deltaTime);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glm::mat4 model, view, projection;

		view = fpsCamera.GetViewMatrix();
		projection = glm::perspective(fpsCamera.getFOV(), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 100.0f);

		PBR_sphereShader.use();
		PBR_sphereShader.setUniform("projection", projection);

		//spot light 
		glm::vec3 spotLight_Pos = fpsCamera.GetPosition();
		glm::vec3 spotLight_Col(1.0f, 1.0f, 1.0f);

		//directional light
		glm::vec3 directional_lightDirection(0.0f, -0.9f, -0.17);
		glm::vec3 directional_lightCol(0.4f, 0.4f, 0.2f);
		
		//point light 
		glm::vec3 point_Pos(0.0f, 0.0f, 0.0f);
		glm::vec3 point_Col(1.0f, 1.0f, 1.0f);
		glm::vec3 point_Direction(0.0f, -0.9f, -0.17);
		glm::vec3 point_Scale(1.2f, 1.2f, 1.2f);
		glm::vec3 point_lightCol(1.0f, 1.0f, 1.0f);

		angle += (float)deltaTime * 90.0f;
		point_Pos.x += 1.5f + 10* sinf(glm::radians(angle));
		point_Pos.z += 1.5f + 10 * cosf(glm::radians(angle));
		point_Pos.y += 3 + (0.5f * sinf(glm::radians(angle) * 4));

		//---------------------------------------------------
		cubemapShader.use();
		glm::vec3 cubeMap_Pos(0.0f, 0.0f, 0.0f);
		glm::vec3 cubeMap_Scale(1.0f, 1.0f, 1.0f);


		cubeMapTexture.bind(0);
		cubemapShader.setUniformSampler("equirectangularMap", 0);
		cubemapShader.setUniform("col", glm::vec4(1.0f, 0.5f, 0.5f, 1.0f));

		model = glm::mat4(1.0f);
		model = glm::translate(model, cubeMap_Pos) * glm::scale(model, cubeMap_Scale);

		//cubemapShader.setUniform("model", model);
		cubemapShader.setUniform("view", view);
		cubemapShader.setUniform("projection", projection);

		CubeMapMesh.Draw();
		//------------------------------------------------


		PBR_sphereShader.use();

		glm::vec3 pbr_Pos(0.0f, 0.0f, 0.0f);
		glm::vec3 pbr_Scale(5.0f, 5.0f, 5.0f);

		glm::vec3 viewPos = fpsCamera.GetPosition();

		PBR_sphereShader.setUniform("view", view);
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

		lightPositions[0] = point_Pos;

		model = glm::mat4(1.0f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f)) * glm::scale(model, pbr_Scale);
		PBR_sphereShader.setUniform("model", model);
				
		PBR_sphereMesh.Draw();

		PBR_sphereTexture[0].unbind(0);
		PBR_sphereTexture[1].unbind(1);
		PBR_sphereTexture[2].unbind(2);
		PBR_sphereTexture[3].unbind(3);
		PBR_sphereTexture[4].unbind(4);
		

		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i){
			glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
			newPos = lightPositions[i];
			std::string str1 = "lightPositions[" + std::to_string(i) + "]";
			const char* c1 = str1.c_str();
			PBR_sphereShader.setUniform(c1, newPos);
			std::string str2 = "lightColors[" + std::to_string(i) + "]";
			const char* c2 = str2.c_str();
			PBR_sphereShader.setUniform(c2, lightColors[i]);

			model = glm::mat4(1.0f);
			model = glm::translate(model, newPos);
			model = glm::scale(model, glm::vec3(0.5f));
			PBR_sphereShader.setUniform("model", model);
			PBR_sphereMesh.Draw();
		}


		
		lightingShader.use();

		lightingShader.setUniform("view"						, view);
		lightingShader.setUniform("projection"					, projection);
		lightingShader.setUniform("viewPos"						, viewPos);

		lightingShader.setUniform("spotLight.position"			, spotLight_Pos);
		lightingShader.setUniform("spotLight.direction"			, fpsCamera.GetLook());
		lightingShader.setUniform("spotLight.ambient"			, glm::vec3(0.25f, 0.5f, 0.2f));
		lightingShader.setUniform("spotLight.diffuse"			, spotLight_Col);
		lightingShader.setUniform("spotLight.specular"			, glm::vec3(1.0f, 1.0f, 1.0f));
		lightingShader.setUniform("spotLight.constant"			, 1.0f);
		lightingShader.setUniform("spotLight.linear"			, 0.07f);
		lightingShader.setUniform("spotLight.exponent"			, 0.017f);
		lightingShader.setUniform("spotLight.cosInnerCone"		, glm::cos(glm::radians(15.0f)));
		lightingShader.setUniform("spotLight.cosOuterCone"		, glm::cos(glm::radians(20.0f)));
		lightingShader.setUniform("spotLight.on"				, gFlashlightOn);

		lightingShader.setUniform("directionalLight.direction"	, directional_lightDirection);
		lightingShader.setUniform("directionalLight.ambient"	, glm::vec3(0.25f, 0.5f, 0.2f));
		lightingShader.setUniform("directionalLight.diffuse"	, directional_lightCol);
		lightingShader.setUniform("directionalLight.specular"	, glm::vec3(1.0f, 1.0f, 1.0f));

		lightingShader.setUniform("pointLight.direction"		, point_Direction);
		lightingShader.setUniform("pointLight.position"			, point_Pos);
		lightingShader.setUniform("pointLight.ambient"			, glm::vec3(0.25f, 0.5f, 0.2f));
		lightingShader.setUniform("pointLight.diffuse"			, point_lightCol);
		lightingShader.setUniform("pointLight.specular"			, glm::vec3(1.0f, 1.0f, 1.0f));
		lightingShader.setUniform("pointLight.constant"			, 1.0f);
		lightingShader.setUniform("pointLight.linear;"			, 0.07f);
		lightingShader.setUniform("pointLight.exponent"			, 0.017f);

		for (int i = 0; i < numModels; i++){
			model = glm::translate(glm::mat4(), modelPos[i]) * glm::scale(glm::mat4(), modelScale[i]);
			lightingShader.setUniform("model", model);
			
			lightingShader.setUniform("material.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
			lightingShader.setUniformSampler("material.diffuseMap", 0);
			lightingShader.setUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
			lightingShader.setUniform("material.shininess", 10.0f);
			
			/*texture[i].bind(0);
			mesh[i].Draw();
			texture[i].unbind(0);*/
		}

		//render PBR mesh
		

		// render light mesh
		model = glm::translate(glm::mat4(), point_Pos) * glm::scale(glm::mat4(), point_Scale);
		lightbulbShader.use();
		lightbulbShader.setUniform("lightCol", point_Col);
		lightbulbShader.setUniform("model", model);
		lightbulbShader.setUniform("view", view);
		lightbulbShader.setUniform("projection", projection);
		lightMesh.Draw();

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
	glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

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

