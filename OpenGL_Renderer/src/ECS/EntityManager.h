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


	void Editor_SetActiveEntity(uint32_t id);
	uint32_t Editor_GetActiveEntity();

	void Editor_RenderActiveInspector(uint32_t id);
	void Editor_RenderActiveInspector();
	void Editor_Reset();

	Entity* FindActiveCamera();
	Entity* GetActiveCamera();

	std::vector<std::unique_ptr<Entity>> entities;

	uint32_t GetSelectedEntityID();
	Entity* GetSelectedEntity();

	std::vector<Entity*> FindLights();
public: // template classes
	template<class T>
	std::vector<Entity*> FindEntitiesWithType() {
		std::vector<Entity*> arr;
		for (auto& entity : entities) {
			if (entity->hasComponent<T>()) {
				if (entity->isActive()) {
					arr.emplace_back(entity.get());
				}
			}
		}
		if (arr.empty()){
			std::cout << "---WARNING : EntityManager.cpp - Could not Find : " << typeid(T).name() << std::endl;
		}

		return arr;
	}

	template<class T>
	Entity* FindEntityWithType() {
		for (auto& entity : entities) {
			if (entity->hasComponent<T>()) {
				if (entity->isActive()) {
					return entity.get();
				}
			}
		}
		std::cout << "---WARNING : EntityManager.cpp - Could not Find : " << typeid(T).name() << std::endl;
		return nullptr;
	}

private:

	uint32_t selectedEntityID;

};

#endif // !ENTITY_H



