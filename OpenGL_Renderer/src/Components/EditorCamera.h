#ifndef EDITORCAMERA_H
#define EDITORCAMERA_H

#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "../ECS/Component.h"
#include "../ECS/Entity.h"

class EditorCamera : public Component
{
public:
	EditorCamera();
	~EditorCamera();

private:

};



#endif // !EDITORCAMERA_H
