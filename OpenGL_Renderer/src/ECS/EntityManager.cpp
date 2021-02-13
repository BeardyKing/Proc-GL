#include "EntityManager.h"
#include "../Engine/ECS_List.h"


void EntityManager::addEntity(Entity* entity){
	std::unique_ptr<Entity> unique_ptr{ entity }; 
	entities.emplace_back(std::move(unique_ptr));
}

void EntityManager::Editor_SetActiveEntity(uint32_t id){
	selectedEntityID = id;
}

Entity* EntityManager::FindActiveCamera() {
	for (auto& entity : entities){
		if (entity->hasComponent<Camera>() || entity->hasComponent<FPSCamera>() || entity->hasComponent<OrbitCamera>()){
			if (entity->isActive()){
				return entity.get();
			}
		}
	}

	std::cout << "---WARNING : EntityManager.cpp - Could not Find Camera in scene" << std::endl;

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

void EntityManager::Editor_RenderActiveInspector(uint32_t id){
	entities[id]->OnImGuiRender();
}

void EntityManager::Editor_RenderActiveInspector() {
	entities[selectedEntityID]->OnImGuiRender();
}

#pragma endregion

#pragma region TO_IMPLEMENT

void EntityManager::refresh(){}
void EntityManager::eraseEntity(Entity* entity){}
Entity* EntityManager::cloneEntity(Entity* entity){return nullptr;}


std::vector<Entity*> EntityManager::FindLights(){
	std::vector<Entity*> arr;
	for (auto& entity : entities) {
		if (entity->hasComponent<LightObject>()) {
			if (entity->isActive()) {
				arr.emplace_back(entity.get());
			}
		}
	}
	return arr;
}



Entity* EntityManager::GetSelectedEntity() {
	return entities[selectedEntityID].get();
}

uint32_t EntityManager::GetSelectedEntityID() {
	return selectedEntityID;
}





#pragma endregion

