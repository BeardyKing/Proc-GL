#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <iostream>
#include <vector>
#include <memory>
#include "../ECS/Entity.h"



class EntityManager
{
public:

	EntityManager() = default;
	~EntityManager() = default;
	
	void OnRender();
	void OnUpdate(double deltaTime);
	void OnImGuiRender();
	void refresh();
	
	void addEntity(Entity* player);
	void eraseEntity(Entity* player);
	Entity* cloneEntity(Entity* player);
private:
	std::vector<std::unique_ptr<Entity>> entities;

};

#endif // !ENTITY_H



