#include <iostream>
#include <sstream>

#define GLEW_STATIC
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include "glm/gtc/matrix_transform.hpp"

#include "ShaderProgram.h"
#include "Texture2D.h"


const char* APP_TITLE = "OpenGL";
int gWindowWidth = 1024;
int gWindowHeight = 768;

GLFWwindow* gWindow = NULL;

bool gFullscreen = false;
bool glWireframe = false;

const std::string texture1Path = "Wall/Brick_wall_02_1K_Base_Color.png";
//const std::string texture2Path = "Wall/Brick_wall_02_1K_Roughness.png";

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_OnFrameBufferSize(GLFWwindow* window, int width, int height);

void showFPS(GLFWwindow* window);
bool InitOpenGL();

int main(){
	if (!InitOpenGL()){ 
		std::cout << "INT FAILED" << std::endl;
		return -1;
	}

	GLfloat verticies[] = {
		//positions				 // text coord ( UV )
		//Front face
		-1.0f,	 1.0f,	1.0f,	 0.0f,	1.0f, 
		 1.0f,	-1.0f,	1.0f,	 1.0f,	0.0f, 
		 1.0f,	 1.0f,	1.0f,	 1.0f,	1.0f, 
		-1.0f,	 1.0f,	1.0f,	 0.0f,	1.0f, 
		-1.0f,	-1.0f,	1.0f,	 0.0f,	0.0f, 
		 1.0f,	-1.0f,	1.0f,	 1.0f,	0.0f, 
										
		 //Back face					
		-1.0f,	 1.0f,	-1.0f,	 0.0f,	1.0f,
		 1.0f,	-1.0f,	-1.0f,	 1.0f,	0.0f,
		 1.0f,	 1.0f,	-1.0f,	 1.0f,	1.0f,
		-1.0f,	 1.0f,	-1.0f,	 0.0f,	1.0f,
		-1.0f,	-1.0f,	-1.0f,	 0.0f,	0.0f,
		 1.0f,	-1.0f,	-1.0f,	 1.0f,	0.0f,
										
		 //Left face					
		-1.0f,	 1.0f,	-1.0f,	 0.0f,	1.0f,
		-1.0f,	-1.0f,	 1.0f,	 1.0f,	0.0f,
		-1.0f,	 1.0f,	 1.0f,	 1.0f,	1.0f,
		-1.0f,	 1.0f,	-1.0f,	 0.0f,	1.0f,
		-1.0f,	-1.0f,	-1.0f,	 0.0f,	0.0f,
		-1.0f,	-1.0f,	 1.0f,	 1.0f,	0.0f,
										
		//Right face					
		 1.0f,	 1.0f,	-1.0f,	 0.0f,	1.0f,
		 1.0f,	-1.0f,	 1.0f,	 1.0f,	0.0f,
		 1.0f,	 1.0f,	 1.0f,	 1.0f,	1.0f,
		 1.0f,	 1.0f,	-1.0f,	 0.0f,	1.0f,
		 1.0f,	-1.0f,	-1.0f,	 0.0f,	0.0f,
		 1.0f,	-1.0f,	 1.0f,	 1.0f,	0.0f,
										
		 //Top face						
		-1.0f,	1.0f,	-1.0f,	 0.0f,	1.0f,
		 1.0f,	1.0f,	 1.0f,	 1.0f,	0.0f,
		 1.0f,	1.0f,	-1.0f,	 1.0f,	1.0f,
		-1.0f,	1.0f,	-1.0f,	 0.0f,	1.0f,
		-1.0f,	1.0f,	 1.0f,	 0.0f,	0.0f,
		 1.0f,	1.0f,	 1.0f,	 1.0f,	0.0f,

		 //Bottom face						
		-1.0f,	-1.0f,	 1.0f,	 0.0f,	1.0f,
		 1.0f,	-1.0f,	-1.0f,	 1.0f,	0.0f,
		 1.0f,	-1.0f,	 1.0f,	 1.0f,	1.0f,
		-1.0f,	-1.0f,	 1.0f,	 0.0f,	1.0f,
		-1.0f,	-1.0f,	-1.0f,	 0.0f,	0.0f,
		 1.0f,	-1.0f,	-1.0f,	 1.0f,	0.0f,
	};

	glm::vec3 cubePos = glm::vec3(0.0f, 0.0f, -5.0f);

	// vbo vertex buffer,
	// ibo index buffer, 
	// vao vertex array obj

	GLuint vbo, vao;
	//GLuint ibo;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// position attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(0);

	// text Coord 

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	/*glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);*/

	ShaderProgram shaderProgram;
	shaderProgram.loadShaders("basic.vert", "basic.frag");

	Texture2D texture1;
	texture1.loadTexture(texture1Path, true);

	//Texture2D texture2;
	//texture2.loadTexture(texture2Path, true);

	float cubeAngle = 0.0f;
	double lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(gWindow)){
		showFPS(gWindow);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;

		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		texture1.bind(0);
		//texture2.bind(1);

		cubeAngle += (float)(deltaTime * 50.0f);
		if (cubeAngle >= 360.0){
			cubeAngle = 0.0f;
		}
		 
		glm::mat4 model, view, projection;
		
		// model starts at 0,0,0 translate it by cube pos 0,0,-5 rotate by cube angle around the 0,1,0 axis 
		model = glm::translate(model, cubePos) * glm::rotate(model, glm::radians(cubeAngle), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::vec3 camPos(0.0f, 0.0f, 0.0f);
		glm::vec3 targetPos(0.0f, 0.0f, -1.0f);
		glm::vec3 up(0.0f, 1.0f, 0.0f);
		view = glm::lookAt(camPos,targetPos,up);

		projection = glm::perspective(glm::radians(45.0f), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 100.0f);
		

		shaderProgram.use();
		/*model;
		view;
		projection*/ 

		shaderProgram.setUniform("model",		model);
		shaderProgram.setUniform("view",		view);
		shaderProgram.setUniform("projection",	projection);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(gWindow);

		lastTime = currentTime;
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	//glDeleteBuffers(1, &ibo);
	glfwTerminate();
	return 0;
}

bool InitOpenGL() {
	if (!glfwInit()){
		std::cerr << "GLFW DID NOT INIT" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cerr << "GLEW Did not init" << std::endl;
		glfwTerminate();
		return false;
	}

	glClearColor(0.25f, 0.38f, 0.47f, 1.0f);
	glViewport(0, 0, gWindowWidth, gWindowHeight);
	glEnable(GL_DEPTH_TEST);

	return true;
}

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_W && action == GLFW_PRESS){
		glWireframe = !glWireframe;
		if (glWireframe){
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
}

void glfw_OnFrameBufferSize(GLFWwindow* window, int width, int height) {
	gWindowWidth	= width;
	gWindowHeight	= height;

	glViewport(0, 0, gWindowWidth, gWindowHeight);
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
