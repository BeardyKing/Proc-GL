#ifndef ENGINE_UTILITYFUNCTIONS_H
#define ENGINE_UTILITYFUNCTIONS_H

#include "../Engine/Engine_Core.h"

extern GLuint G_GetCamDepth();
extern void G_SetCamDepth(GLuint tex);

extern void G_SetRenderShadowMap(bool b);
extern GLuint G_GetShadowMap();
extern void G_SetShadowMap(GLuint tex);
extern bool G_RenderShadowMap();

extern EntityManager* G_GetManager();
extern uint32_t GetAmountOfEntities();
extern void G_SetManager(EntityManager* mgr);


#endif // !ENGINE_UTILITYFUNCTIONS_H

