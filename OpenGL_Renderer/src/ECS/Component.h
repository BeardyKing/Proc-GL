#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;

class Component
{
public:
	Component() = default;
	Component(Entity& e);
	
	virtual ~Component() = default;

	Entity* entity = nullptr;



	virtual bool init();

	virtual void OnRender();
	virtual void OnUpdate(double deltaTime);
	virtual void OnImGuiRender();
	virtual void OnExit();

};

#endif // !COMPONENT_H
