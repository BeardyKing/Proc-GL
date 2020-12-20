#ifndef TEST_ECS_H
#define TEST_ECS_H
#include "test.h"

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


namespace test {
	class test_ECS : public Test
	{
	public:
		test_ECS();
		~test_ECS();

	private:
		void OnUpdate(double deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnExit()override;

		void RenderHierarchy();
	
	private:
		Entity* entity;
	};

}
	//static EntityManager* g_ecs_manager;

#endif // !TEST_ECS_H
