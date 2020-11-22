#include "EntityManager.h"



void EntityManager::draw()
{
	for (auto& entity : entities){
		entity->draw();
	}

}

void EntityManager::update(){
	for (auto& entity : entities) {
		entity->update();
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
