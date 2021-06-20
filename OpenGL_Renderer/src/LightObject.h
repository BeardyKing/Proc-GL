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

	glm::mat4 LightSpaceMatrix();

private:

	
public:
	enum LightType{ Point, Directional, Spot};
	LightType lightType;
	glm::vec3 color = glm::vec3(1);
	glm::vec3 lookAtTarget = glm::vec3(0);
	glm::vec3 shadowColour = glm::vec3(1);
	float shadowIntensity = 0.5f;
	float lightIntensity = 0.75f;
	float shadowBias = 1.0f;
	glm::vec4 v4lightProjection = glm::vec4(-15.0f, 15.0f, -15.0f, 15.0f);
	float near_plane = 0.01f;
	float far_plane = 35.0f;
};

#endif // !LIGHTOBJECT_H


