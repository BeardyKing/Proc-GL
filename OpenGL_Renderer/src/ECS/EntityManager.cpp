#include "EntityManager.h"


void EntityManager::addEntity(Entity* entity){
	std::unique_ptr<Entity> unique_ptr{ entity }; 
	entities.emplace_back(std::move(unique_ptr));
}

void EntityManager::Editor_SetActiveEntity(uint32_t id){
	selectedEntityID = id;
}

#pragma region CALLBACKS

void EntityManager::OnRender(){
	for (auto& entity : entities){
		entity->OnRender();
	}
}

void EntityManager::OnUpdate(double deltaTime){
	for (auto& entity : entities) {
		entity->OnUpdate(deltaTime);
	}
}

void EntityManager::OnImGuiRender() {
	for (auto& entity : entities) {
		entity->OnImGuiRender();
	}
}

void EntityManager::OnExit() {
	for (auto& entity : entities) {
		entity->OnExit();
		entity.release();
	}
}

#pragma endregion

#pragma region EDITOR

uint32_t EntityManager::Editor_GetActiveEntity(){
	return selectedEntityID;
}

void EntityManager::Editor_RenderActiveEditityGui(uint32_t id){
	entities[id]->OnImGuiRender();
}

void EntityManager::Editor_RenderActiveEditityGui() {
	entities[selectedEntityID]->OnImGuiRender();
}

#pragma endregion

#pragma region TO_IMPLEMENT

void EntityManager::refresh(){}
void EntityManager::eraseEntity(Entity* entity){}
Entity* EntityManager::cloneEntity(Entity* entity){return nullptr;}

Entity* EntityManager::FindEntityWithType() {
	/*for (auto& entity : entities){
		if (entities[i]->hasComponent<T>()){
			return entities[i];
		}
	}*/	
	return NULL;
}



#pragma endregion

