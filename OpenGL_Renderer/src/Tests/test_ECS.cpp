#include "test_ECS.h"

namespace test {
	test_ECS::test_ECS(){
		editor = new EditorGUI;

		EntityManager* manager = new EntityManager;
		SetManager(manager);

		entity = new Entity("Main Camera");
		entity->addComponent<FPSCamera>();
		auto& cam = entity->getComponent<FPSCamera>().usingImGuiWindow = false;
		entity->getComponent<Transform>().position = glm::vec3(0, 0, -18);
		GetManager()->addEntity(entity);

		for (size_t i = 0; i < 100; i++){
			std::string name = "basic sphere ";
			name.append(std::to_string(i));
			entity = new Entity(name.c_str());
			GetManager()->addEntity(entity);
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
		GetManager()->addEntity(entity);

		entity = new Entity("light");
		entity->addComponent<LightObject>();
		GetManager()->addEntity(entity);

	}

	test_ECS::~test_ECS(){

	}

	void test_ECS::OnUpdate(double deltaTime) {
		GetManager()->OnUpdate(deltaTime);
	}

	void test_ECS::OnRender() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GetManager()->OnRender();
	}

	void test_ECS::OnImGuiRender() {
		editor->RenderHierarchy();
		GetManager()->Editor_RenderActiveInspector();
	}

	void test_ECS::OnExit(){
		GetManager()->OnExit();
	}
}