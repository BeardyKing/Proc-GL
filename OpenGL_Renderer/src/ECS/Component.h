#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;

class Component
{
public:
	Component() = default;
	~Component() = default;

	Entity* entity;

	virtual bool init() {
		return true;
	}

	virtual void draw() {

	}

	virtual void update() {

	}

private:

};

#endif // !COMPONENT_H
