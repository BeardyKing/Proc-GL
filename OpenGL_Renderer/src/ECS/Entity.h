#ifndef ENTITY_H
#define ENITITY_H

#include "ECS.h"
#include <vector>
#include <memory>

#include "Component.h"

class Entity
{
public:
	Entity() {}
	virtual ~Entity() {}

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

private:
	ComponentList compList;
	ComponentBitset compBitset;

	std::vector<std::unique_ptr<Component>> components;

};



#endif // !ENTITY_H

