#include "test_ECS.h"

namespace test {
	test_ECS::test_ECS(){
		editor = new EditorGUI;

		EntityManager* manager = new EntityManager;
		G_SetManager(manager);

		entity = new Entity("Main Camera");
		entity->addComponent<FPSCamera>();
		auto& cam = entity->getComponent<FPSCamera>().usingImGuiWindow = false;
		entity->getComponent<Transform>().position = glm::vec3(0, 0, -18);
		G_GetManager()->addEntity(entity);

		for (size_t i = 0; i < 100; i++){
			std::string name = "basic sphere ";
			name.append(std::to_string(i));
			entity = new Entity(name.c_str());
			G_GetManager()->addEntity(entity);
			entity->addComponent<ShaderProgram>();
			entity->getComponent<ShaderProgram>().SetBaseColor(glm::vec3(glm::ballRand((float)i / 2)));
			if (i % 2){
				entity->addComponent<Mesh>("objectDefaults/cube.obj");
			}
			else if(i % 3){
				entity->addComponent<Mesh>("mesh/pipe.obj");
			}
			else{
				entity->addComponent<Mesh>();
			}
			entity->getComponent<Transform>().position = glm::vec3(glm::ballRand((float)i/2));
			entity->getComponent<Transform>().rotation = glm::vec3(glm::ballRand((float)i/2));
		}

		entity = new Entity();
		G_GetManager()->addEntity(entity);

		entity = new Entity("light");
		entity->addComponent<LightObject>();
		G_GetManager()->addEntity(entity);

	}

	test_ECS::~test_ECS(){

	}

	void test_ECS::OnUpdate(double deltaTime) {
		G_GetManager()->OnUpdate(deltaTime);
	}

	void test_ECS::OnRender() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		G_GetManager()->OnRender();
	}

	void test_ECS::OnImGuiRender() {
		editor->RenderHierarchy();
		G_GetManager()->Editor_RenderActiveInspector();
	}

	void test_ECS::OnExit(){
		G_GetManager()->OnExit();
	}
}