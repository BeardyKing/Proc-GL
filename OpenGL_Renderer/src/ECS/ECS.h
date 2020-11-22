#ifndef ECS_H
#define ECS_H

#include <array>
#include <bitset>
#include <iostream>

class Entity;
class Component;

using ComponentTypeID = std::size_t;

inline ComponentTypeID getUniqueComponentID() {
	static ComponentTypeID lastID = 0u;
	return lastID++;
}

template<typename T>
inline ComponentTypeID getComponentTypeID() noexcept{
	static_assert(std::is_base_of <Component, T>::value, "Type not base on component!");
	static const ComponentTypeID typeID = getUniqueComponentID();
	return typeID;
}

constexpr std::size_t MAX_ENTITIES = 9000;
constexpr std::size_t MAX_COMPONENTS = 64;

using ComponentBitset = std::bitset< MAX_COMPONENTS>;
using ComponentList = std::array<Component*, MAX_COMPONENTS>;

#endif // !ECS_H
