#ifndef ENTITY_H
#define ENITITY_H

#include "ECS.h"
#include <vector>
#include <memory>

#include "Component.h"
#include "../Components/Transform.h"

class Entity
{
public:
	Entity() {
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

	inline bool isActive() const {
		return active;
	}

	inline void destroy() {
		active = false;
	}

	inline void draw() {
		for (auto& comp : components) {
			comp->draw();
		}
	}

	inline void update() {
		for (auto& comp : components) {
			comp->update();
		}
	}

private:
	bool active = true;
	ComponentList compList;
	ComponentBitset compBitset;

	std::vector<std::unique_ptr<Component>> components;

};



#endif // !ENTITY_H

