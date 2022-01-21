#ifndef OBJECTDATA_H
#define OBJECTDATA_H

#include "../ECS/Component.h"
#include "../vendor/imgui/imgui.h"

#include <memory>
#include <string>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

static uint32_t G_idCounter = 0;


class ObjectData : public Component{
public:
	ObjectData();
	ObjectData(char* _name);
	ObjectData(std::string _name);

	~ObjectData();

	void OnRender();
	void OnUpdate(double deltaTime);
	void OnImGuiRender();
	void OnExit();

	void SetName(char* _name);

	const char* GetName();
	
	uint32_t GetID();

private:
	std::string name;
	uint32_t entity_id;
};
#endif // !OBJECTDATA_H


