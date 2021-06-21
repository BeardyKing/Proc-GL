#ifndef ECS_LIST_H
#define ECS_LIST_H

//#include "../ECS/Entity.h"
//#include "../ECS/EntityManager.h"
//
//#include "../Engine/Engine_Core.h"

#include "../Components/Transform.h"
#include "../Components/ObjectData.h"

#include "../Components/scripts/script_simpleBehaviours.h"

#include "../Components/LightObject.h"
#include "../Components/Camera.h"




extern uint32_t GetAmountOfEntities();
extern void SetManager(EntityManager* mgr);
EntityManager* GetManager();

#endif // !ECS_LIST_H
