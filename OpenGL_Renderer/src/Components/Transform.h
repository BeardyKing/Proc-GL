#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/gtc/type_ptr.hpp>
#include "../ECS/Component.h"

class Transform : public Component
{
public:
	Transform() :
		position(glm::vec3(0)),
		rotation(glm::vec3(0)),
		scale(glm::vec3(1))
	{}


	Transform(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale) {
		position = _position;
		rotation = _rotation;
		scale = _scale;
	}

	~Transform() = default;

public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};



#endif // !TRANSFORM_H



