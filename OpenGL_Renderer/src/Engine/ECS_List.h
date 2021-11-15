#ifndef ECS_LIST_H //TODO RENAME TO COMPONENT LIST
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
#include "../Components/SkyBox.h"




extern uint32_t GetAmountOfEntities();
extern void G_SetManager(EntityManager* mgr);
EntityManager* G_GetManager();

#endif // !ECS_LIST_H
