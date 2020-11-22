#include "EntityManager.h"



void EntityManager::OnRender()
{
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



void EntityManager::refresh()
{
}

void EntityManager::addEntity(Entity* player){
	std::unique_ptr<Entity> unique_ptr{ player }; // player not entity
	entities.emplace_back(std::move(unique_ptr));
}

void EntityManager::eraseEntity(Entity* player)
{

}

Entity* EntityManager::cloneEntity(Entity* player)
{
	return nullptr;
}
