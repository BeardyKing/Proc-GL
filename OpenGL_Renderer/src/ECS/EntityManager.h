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
	void OnExit();
	
	void addEntity(Entity* player);
	void eraseEntity(Entity* player);
	Entity* cloneEntity(Entity* player);

	Entity* FindEntityWithType();

	void Editor_SetActiveEntity(uint32_t id);
	uint32_t Editor_GetActiveEntity();

	void Editor_RenderActiveInspector(uint32_t id);
	void Editor_RenderActiveInspector();
	void Editor_Reset();

	std::vector<std::unique_ptr<Entity>> entities;

	uint32_t GetSelectedEntityID();
	Entity* GetSelectedEntity();
private:

	uint32_t selectedEntityID;

};

#endif // !ENTITY_H



