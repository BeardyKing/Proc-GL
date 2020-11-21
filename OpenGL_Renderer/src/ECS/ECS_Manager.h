#ifndef ECS_MANAGER_H
#define ECS_MANAGER_H
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iostream>

class ECS_Manager
{
public:
	ECS_Manager();
	~ECS_Manager();
    using EntityID = int64_t; //for this post's purpose, int64_t is an arbitrary choice

    EntityID entitys;

	void Update(double deltaTime);
    struct Position
    {
        float x;
        float y;
    };

    struct Velocity
    {
        float x;
        float y;
    };

    struct Health
    {
        int max;
        int current;
    };

    template <typename Type>
    using ComponentMap = std::unordered_map<EntityID, Type>;

    using Positions = ComponentMap<Position>;
    using Velocities = ComponentMap<Velocity>;
    using Healths = ComponentMap<Health>;

    struct Components
    {
        Positions positions;
        Velocities velocities;
        Healths healths;
    };

    void updateHealthRegeneration(int64_t currentFrame, Healths& healths);




private:
};

#endif // !ECS_MANAGER_H


