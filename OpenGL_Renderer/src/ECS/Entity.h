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

	Entity(const char* _name) {
		this->addComponent<ObjectData>(_name);
		this->addComponent<Transform>();
	};

	virtual ~Entity() = default;

	template<typename T, typename... TArgs>
	inline T& addComponent(TArgs&&... args) {
		T* comp(new T(std::forward<TArgs>(args)...));

		std::unique_ptr<Component> uptr{ comp };
		components.emplace_back( std::move(uptr) );

		if (comp->init()){
			compList[getComponentTypeID<T>()] = comp;
			compBitset[getComponentTypeID<T>()] = true;
			comp->entity = this;
			return *comp;
		}

		return *static_cast<T*>(nullptr);
	}

	template<typename T>
	inline T& getComponent() const {
		auto ptr(compList[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}

	template<typename T>
	inline bool hasComponent() const {
		return compBitset[getComponentTypeID<T>()];
	}

	template<typename T>
	inline void removeComponent() {
		if (hasComponent<T>()){			
			auto target = compList[getComponentTypeID<T>()];
			for (size_t i = 0; i < components.size(); i++){
				if (components[i].get() == target){
					components.erase(components.begin() + i);
				}
			}
		}
		//std::cout << "Removed component" << std::endl;
	}

	inline bool isActive() const {
		return active;
	}

	inline void destroy() {
		active = false;
	}

	inline void OnRender() {
		for (auto& comp : components) {
			comp->OnRender();
		}
	}

	inline void OnUpdate(double deltaTime) {
		for (auto& comp : components) {
			comp->OnUpdate(deltaTime);
		}
	}

	inline void OnExit() {
		for (auto& comp : components) {
			comp->OnExit();
		}
	}

	inline void OnImGuiRender() {
		for (auto& comp : components){
			comp->OnImGuiRender();
		}
	}

	inline ComponentBitset GetBitSet() {
		return compBitset;
	}

private:
	bool active = true;
	ComponentList compList;
	ComponentBitset compBitset;

	std::vector<std::unique_ptr<Component>> components;
};

#endif // !ENTITY_H

