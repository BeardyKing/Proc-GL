#ifndef COMPONENT_H
#define COMPONENT_H

extern class Entity;

class Component
{
public:
	Component() = default;

	Component(Entity& e):
		entity(&e)
	{
	}
	~Component() = default;

	Entity* entity;

	virtual bool init() {
		return true;
	}

	virtual void OnRender() {

	}

	virtual void OnUpdate(double deltaTime) {

	}

	virtual void OnImGuiRender() {

	}

private:

};

#endif // !COMPONENT_H
