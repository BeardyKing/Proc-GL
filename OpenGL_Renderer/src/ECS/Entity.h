#ifndef ENTITY_H
#define ENTITY_H

#include "ECS.h"
#include <vector>
#include <memory>

#include "Component.h"
#include "../Components/ObjectData.h"
#include "../Components/Transform.h"

class Entity
{
public:
	Entity() {
		this->addComponent<ObjectData>();
		this->addComponent<Transform>();
	};

	Entity(const char* name) {
		this->addComponent<ObjectData>(name);
		this->addComponent<Transform>();
	};

	virtual ~Entity() {
		//destroy();
		
	}



	template<typename T, typename... TArgs>
	inline T& addComponent(TArgs&&... args) {
		T* new_component(new T(std::forward<TArgs>(args)...));
		components.emplace_back( new_component );

		if (new_component->init()){
			component_array[getComponentTypeID<T>()] = new_component;
			component_bitset[getComponentTypeID<T>()] = true;
			new_component->entity = this;
			return *new_component;
		}

		return *static_cast<T*>(nullptr);
	}

	template<typename T>
	inline T& getComponent() const {
		auto ptr(component_array[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}

	template<typename T>
	inline bool hasComponent() const {
		return component_bitset[getComponentTypeID<T>()];
	}

	template<typename T>
	inline void removeComponent() {
		if (hasComponent<T>()){			
			auto target = component_array[getComponentTypeID<T>()];
			for (size_t i = 0; i < components.size(); i++){
				if (components[i].get() == target){
					components.erase(components.begin() + i);
				}
			}
		}
	}

	inline bool isActive() const {
		return active;
	}

	inline void isActive(bool setActive){
		active = setActive;
	}

	inline bool* isActiveRawPtr(){
		return &active;
	}

	inline void destroy() {
		std::cout << "destroyed entity" << std::endl;
		components.clear();
		//active = false; // TODO remove from ECS instead of set active to false
	}

	inline void OnRender() {
		if (!active){
			return;
		}
		for (auto& component : components) {
			component->OnRender();
		}
	}

	inline void OnUpdate(double deltaTime) {
		if (!active) {
			return;
		}
		for (auto& component : components) {
			component->OnUpdate(deltaTime);
		}
	}

	inline void OnExit() {
		for (auto& component : components) {
			component->OnExit();
		}
	}

	inline void OnImGuiRender() {
		for (auto& component : components){
			component->OnImGuiRender();
		}
	}

	inline ComponentBitset GetBitSet() {
		return component_bitset;
	}

private:
	bool active = true;
	ComponentList component_array;
	ComponentBitset component_bitset;

	std::vector<std::unique_ptr<Component>> components;
};

#endif // !ENTITY_H

