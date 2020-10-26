#include "test_PBR.h"

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

namespace test {
	test_PBR::test_PBR() :
		m_fpsCamera(glm::vec3(0.0f, 0.0f, 18.0f)),
		m_amountOfLights(6),
		m_pbrSpherePosition(glm::vec3(0)),
		m_pbrSphereRotation(glm::vec3(-180, 0, 0)),
		m_pbrSphereScale(glm::vec3(5)),
		m_pbrSphereRotationSpeed(0.0f)
	{

		m_PBR_sphereMesh = std::make_unique<Mesh>();
		m_PBR_sphereMesh->LoadOBJ("sphere.obj");

		m_PBR_sphereShader = std::make_unique<ShaderProgram>();
		m_PBR_sphereShader->loadShaders("pbr.vert", "pbr.frag");

		m_PBR_sphereShader->use();
		m_PBR_sphereShader->setUniform("albedo", glm::vec3(0.5f, 0.0f, 0.0f));
		m_PBR_sphereShader->setUniform("ao", 1.0f);

		m_PBR_sphereTexture = std::make_unique<Texture2D[]>(5);
		m_PBR_sphereTexture[0].loadTexture("paint/Painted_metal_02_1K_Base_Color.png", true);	//albedo
		m_PBR_sphereTexture[1].loadTexture("paint/Painted_metal_02_1K_Normal.png", true);	//normal
		m_PBR_sphereTexture[2].loadTexture("paint/Painted_metal_02_1K_Metallic.png", true);	//metalic
		m_PBR_sphereTexture[3].loadTexture("paint/Painted_metal_02_1K_Roughness.png", true);	//roughness
		m_PBR_sphereTexture[4].loadTexture("paint/Painted_metal_02_1K_AO.png", true);	//ambient occlusion 

		//----------------------------------//
		//			POINT LIGHTS			//
		//----------------------------------//

		m_pointLights = std::make_unique<LightObject[]>(m_amountOfLights);

		m_pointLights[0].position = glm::vec3(0.0f, 0.0f, 0.0f);
		m_pointLights[1].position = glm::vec3(10.0f, 10.0f, 10.0f);
		m_pointLights[2].position = glm::vec3(-13.0f, 0.0f, 10.0f);
		m_pointLights[3].position = glm::vec3(10.0f, -10.0f, 10.0f);
		m_pointLights[4].position = glm::vec3(-13.0f, 10.0f, 10.0f);
		m_pointLights[5].position = glm::vec3(-13.0f, 10.0f, -10.0f);

		//----------------------------------//
		float angle = 10.0f;
		//----------------------------------//


		for (size_t i = 0; i < m_amountOfLights; i++)
		{
			std::cout << m_pointLights[i].position.x << std::endl;
			std::cout << m_pointLights[i].position.y << std::endl;
			std::cout << m_pointLights[i].position.x << std::endl;
			std::cout << "------------------" << std::endl;
		}
	}

	test_PBR::~test_PBR() {}
	void test_PBR::OnUpdate(double deltaTime){
		float MOVE_SPEED = 5;

		m_movingLightAngle += (float)deltaTime * 90.0f;	// rotate lights

		if (ImGui::IsKeyDown('E') && m_mouseFlag == false) { m_mouseEnabled = !m_mouseEnabled; m_mouseFlag = true; }
		if (ImGui::IsKeyReleased('E')) {m_mouseFlag = false; }
		if (m_mouseEnabled) {return;}

		if (m_ignoreForXFrames > 0){
			m_ignoreForXFrames--;
			if (m_ignoreForXFrames == 0){
				m_lastMousePos.x = ImGui::GetMousePos().x;
				m_lastMousePos.y = ImGui::GetMousePos().y;
			}
			return;
		}
		
		//mouse 
		glm::vec2 currentMousePos;

		currentMousePos.x = ImGui::GetMousePos().x;
		currentMousePos.y = ImGui::GetMousePos().y;

		glm::vec2 mouseDelta = m_lastMousePos - currentMousePos;
		m_fpsCamera.Rotate(mouseDelta.x * deltaTime * 30, mouseDelta.y * deltaTime * 30);
		
		// keyboard
		if (ImGui::IsKeyDown('W')){w = true;}
		if (ImGui::IsKeyDown('A')){a = true;}
		if (ImGui::IsKeyDown('S')){s = true;}
		if (ImGui::IsKeyDown('D')){d = true;}
		if (ImGui::IsKeyDown('L')){d = true;}
		if (ImGui::IsKeyDown(GLFW_KEY_LEFT_SHIFT)){l_shift = true;}

		if (ImGui::IsKeyReleased('W')) { w = false; }
		if (ImGui::IsKeyReleased('A')) { a = false; }
		if (ImGui::IsKeyReleased('S')) { s = false; }
		if (ImGui::IsKeyReleased('D')) { d = false; }
		if (ImGui::IsKeyReleased(GLFW_KEY_LEFT_SHIFT)){l_shift = false;}
			
		float moveSpeedDelta = 1;
		if (l_shift) {
			moveSpeedDelta = 4;
		}

		if (w)		{m_fpsCamera.Move((MOVE_SPEED * moveSpeedDelta) * (float)deltaTime * m_fpsCamera.GetLook());}
		else if (s) {m_fpsCamera.Move((MOVE_SPEED * moveSpeedDelta) * (float)deltaTime * -m_fpsCamera.GetLook());}

		if (a)		{m_fpsCamera.Move((MOVE_SPEED * moveSpeedDelta) * (float)deltaTime * -m_fpsCamera.GetRight());}
		else if (d)	{m_fpsCamera.Move((MOVE_SPEED * moveSpeedDelta) * (float)deltaTime * m_fpsCamera.GetRight());}

		m_lastMousePos = currentMousePos;
	}

	void test_PBR::OnRender() {
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//----------------------------------//
		//				MVP					//
		//----------------------------------//
		glm::mat4 model, view, projection;
		view = m_fpsCamera.GetViewMatrix();
		projection = glm::perspective(m_fpsCamera.getFOV(), (float)1024 / (float)768, 0.1f, 100.0f);
		
		//----------------------------------//
		glm::vec3 viewPos = m_fpsCamera.GetPosition();
		//----------------------------------//

		//----------------------------------//
		//			PBR MATERIAL			//
		//----------------------------------//

		m_PBR_sphereShader->use();

		model = glm::mat4(1.0f);
		model = glm::translate(model, m_pbrSpherePosition) * glm::scale(model, m_pbrSphereScale) * glm::rotate(model, (float)glfwGetTime() * m_pbrSphereRotationSpeed, m_pbrSphereRotation);

		m_PBR_sphereShader->setUniform("model", model);
		m_PBR_sphereShader->setUniform("view", view);
		m_PBR_sphereShader->setUniform("projection", projection);
		m_PBR_sphereShader->setUniform("camPos", viewPos);

		m_PBR_sphereTexture[0].bind(0);
		m_PBR_sphereTexture[1].bind(1);
		m_PBR_sphereTexture[2].bind(2);
		m_PBR_sphereTexture[3].bind(3);
		m_PBR_sphereTexture[4].bind(4);

		m_PBR_sphereShader->setUniformSampler("albedoMap",		0);
		m_PBR_sphereShader->setUniformSampler("normalMap",		1);
		m_PBR_sphereShader->setUniformSampler("metallicMap",	2);
		m_PBR_sphereShader->setUniformSampler("roughnessMap",	3);
		m_PBR_sphereShader->setUniformSampler("aoMap",			4);
						
		m_PBR_sphereMesh->Draw();

		//----------------------------------//
		//		PBR SET LIGHT UNIFORMS		//
		//----------------------------------//		

		m_PBR_sphereShader->use();
		m_PBR_sphereShader->setUniform("amountOfLights", m_amountOfLights);

		for (unsigned int i = 0; i < m_amountOfLights; ++i) {
			glm::vec3 newPos = m_pointLights[i].position + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
			newPos = m_pointLights[i].position;

			std::string str1 = "lightPositions[" + std::to_string(i) + "]";
			const char* c1 = str1.c_str();
			m_PBR_sphereShader->setUniform(c1, newPos);

			std::string str2 = "lightColors[" + std::to_string(i) + "]";
			const char* c2 = str2.c_str();
			m_PBR_sphereShader->setUniform(c2, m_pointLights[i].color);
		}

		//----------------------------------//
		//			Moving Light			//
		//----------------------------------//

		m_pointLights[0].position.x = 1.5f + 10 * sinf(glm::radians(m_movingLightAngle));
		m_pointLights[0].position.z = 1.5f + 10 * cosf(glm::radians(m_movingLightAngle));
		m_pointLights[0].position.y = 3 + (0.5f * sinf(glm::radians(m_movingLightAngle) * 4));

		//----------------------------------//
		//			Render Lights			//
		//----------------------------------//

		for (unsigned int i = 0; i < m_amountOfLights; i++) {
			model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(0.5f));
			model = glm::translate(model, m_pointLights[i].position);

			m_pointLights[i].m_Shader->use();
			m_pointLights[i].m_Shader->setUniform("lightCol", m_pointLights[i].color);
			m_pointLights[i].m_Shader->setUniform("model", model);
			m_pointLights[i].m_Shader->setUniform("view", view);
			m_pointLights[i].m_Shader->setUniform("projection", projection);
			m_pointLights[i].m_Mesh->Draw();
		}
	}

	void test_PBR::OnImGuiRender()
	{
		ImGui::Begin("TRANSFROM");
		ImGui::SliderFloat3("PBR SPHERE POSITION : ", &m_pbrSpherePosition.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("PBR SPHERE ROTATION : ", &m_pbrSphereRotation.x, -180.0f, 180.0f);
		ImGui::SliderFloat3("PBR SPHERE SCALE : ", &m_pbrSphereScale.x, 0.1f, 10.0f);
		ImGui::SliderFloat("PBR SPHERE ROTATION SPEED : ", &m_pbrSphereRotationSpeed, -5.0f, 5.0f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
}