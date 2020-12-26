#ifndef TEST_PBR_H
#define TEST_PBR_H

#include "test.h"
#include <memory>

#include "../LightObject.h"
#include "../ShaderProgram.h"
#include "../Texture2D.h"
#include "../Mesh.h"
#include "../Camera.h"
#include "../Components/Transform.h"
#include "../Components/ObjectData.h"
#include "../ECS/Entity.h"
#include "../ECS/EntityManager.h"

#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/random.hpp"

#include "../LightObject.h"
#include "../Mesh.h"
#include "../ShaderProgram.h"
#include "../Camera.h"
#include "../Components/ObjectData.h"

int G_GetWindowWidth();
int G_GetWindowHeight();

namespace test {
	class test_PBR : public Test
	{
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
		std::unique_ptr <LightObject[]> m_pointLights;
		
		int m_amountOfLights = 6;

		//FPSCamera m_fpsCamera;

		Transform m_pbrTransform;
		/*glm::vec3 m_pbrSpherePosition;
		glm::vec3 m_pbrSphereRotationAxis;
		glm::vec3 m_pbrSphereScale;*/

		float m_movingLightAngle = 90;

		glm::vec2 m_lastMousePos;
		bool m_mouseEnabled = true;
		bool m_mouseFlag = false;

		int m_ignoreForXFrames = 1;

		bool w, a, s, d, l_shift;
		
		Entity* entity;
		FPSCamera* camRef;	
	};
}

#endif // !TEST_PBR_H
