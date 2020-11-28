#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;

class Component
{
public:
	Component() = default;
	Component(Entity& e);
	
	~Component() = default;

	Entity* entity;



	virtual bool init();

	virtual void OnRender();
	virtual void OnUpdate(double deltaTime);
	virtual void OnImGuiRender();
	virtual void OnExit();



private:

};

#endif // !COMPONENT_H
