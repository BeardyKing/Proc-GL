#ifndef TEST_PBR_H
#define TEST_PBR_H

#include "test.h"
#include <memory>
#include <iostream>
#include <sstream>
#include <vector>

#include "../Engine/ECS_List.h"
#include "../Engine/EditorGUI.h"

#include "../Texture2D.h"
#include "../ShaderProgram.h"

int G_GetWindowWidth();
int G_GetWindowHeight();

extern uint32_t GetAmountOfEntities();
extern void SetManager(EntityManager* mgr);
EntityManager* GetManager();

namespace test {
	class test_PBR : public Test{
	public:
		test_PBR();
		~test_PBR();

		void OnUpdate(double deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnExit()override;

		void RenderHierarchy();

	private:

		std::unique_ptr <Mesh> m_PBR_sphereMesh;
		std::unique_ptr <ShaderProgram> m_PBR_sphereShader;
		std::unique_ptr <Texture2D[]> m_PBR_sphereTexture;
		std::vector <Entity*> m_point_lights;		

		Entity* m_pbr_cube;
		Entity* entity;
		FPSCamera* camRef;

		int m_amountOfLights = 6;
		float m_movingLightAngle = 90;

	};
}

#endif // !TEST_PBR_H
