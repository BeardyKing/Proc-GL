#ifndef OBJECTDATA_H
#define OBJECTDATA_H

#include "../ECS/Component.h"
#include <memory>
#include <iostream>
#include <string>
#include "../vendor/imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

class ObjectData : public Component
{
public:
	ObjectData();
	ObjectData(char* _name);
	ObjectData(std::string _name);

	~ObjectData();

	void OnRender();
	void OnUpdate(double deltaTime);
	void OnImGuiRender();

	void SetName(char* _name);

	const char* GetName();

	uint32_t GetID();

private:
	std::string name;
	uint32_t entity_id;
};


#endif // !OBJECTDATA_H


