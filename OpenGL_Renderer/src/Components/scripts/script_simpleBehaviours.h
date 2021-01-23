#ifndef SCRIPT_SIMPLEBEHAVIOURS
#define SCRIPT_SIMPLEBEHAVIOURS

#include "../../ECS/Component.h"
#include "../../Engine/ECS_List.h"
#include "../../vendor/imgui/imgui.h"

#include <memory>
#include <iostream>
#include <string>


class script_simplebehaviours : public Component
{
public:
	script_simplebehaviours();

	~script_simplebehaviours();

	void OnRender();
	void OnUpdate(double deltaTime);
	void OnImGuiRender();
	void OnExit();


private:

};




#endif // !SCRIPT_SIMPLEBEHAVIOURS
