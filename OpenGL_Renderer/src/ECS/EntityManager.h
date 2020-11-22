#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <iostream>
#include <vector>
#include <memory>
#include "../ECS/Entity.h"

extern class Entity;

class EntityManager
{
public:

	EntityManager() = default;
	~EntityManager() = default;
	
	void draw();
	void update();
	void refresh();
	
	void addEntity(Entity* player);
	void eraseEntity(Entity* player);
	Entity* cloneEntity(Entity* player);
private:
	std::vector<std::unique_ptr<Entity>> entities;

};

#endif // !ENTITY_H



