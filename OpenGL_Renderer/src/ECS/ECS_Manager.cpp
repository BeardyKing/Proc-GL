#include "ECS_Manager.h"
#include <bitset>


ECS_Manager::ECS_Manager() :
    entitys(0)
{
    std::cout << "HELLO" << std::endl;
    //given a Components instance c
    Components c;
    for (int i = 0; i < 30; i++)
    {
        EntityID newID = ++entitys;
        c.positions[newID] = Position{ 0.0f, 0.0f };
        c.healths[newID] = Health{ 100 + i, 100 + i };
    }


    
}


void ECS_Manager::updateHealthRegeneration(int64_t currentFrame, Healths& healths)
{

        for (auto& health : healths)
        {
            std::cout << health.second.current << std::endl;
            /*if (health.second.current < health.second.max)
                ++health.second.current;*/
        }
        
    
}


ECS_Manager::~ECS_Manager() {

}

void ECS_Manager::Update(double deltaTime) {
    
}