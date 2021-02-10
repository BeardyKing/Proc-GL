#ifndef LIGHTOBJECT_H
#define LIGHTOBJECT_H

#include <memory>

#include "ECS/Component.h"
#include "Components/Transform.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "glm/glm.hpp"

class LightObject : public Component{
public:
	LightObject();
	~LightObject();

	void OnRender();
	void OnUpdate(double deltaTime);
	void OnImGuiRender();

private:
	const char* m_vertexShaderPath;
	const char* m_fragmentShaderPath;
	const char* m_meshPath;

public:
	enum LightType{ Point, Directional, Spot};
	LightType lightType;
	glm::vec3 color;
};

#endif // !LIGHTOBJECT_H


