#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/gtc/type_ptr.hpp>

class Transform
{
public:
	Transform();
	~Transform();

public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};



#endif // !TRANSFORM_H



