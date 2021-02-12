#ifndef ECS_LIST_H
#define ECS_LIST_H

#include "../ECS/Entity.h"
#include "../ECS/EntityManager.h"

#include "../Components/Transform.h"
#include "../Components/ObjectData.h"

#include "../Components/scripts/script_simpleBehaviours.h"

#include "../LightObject.h"
#include "../Mesh.h"
#include "../ShaderProgram.h"
#include "../Camera.h"


extern uint32_t GetAmountOfEntities();
extern void SetManager(EntityManager* mgr);
EntityManager* GetManager();

#endif // !ECS_LIST_H
