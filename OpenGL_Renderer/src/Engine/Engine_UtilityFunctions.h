#ifndef ENGINE_UTILITYFUNCTIONS_H
#define ENGINE_UTILITYFUNCTIONS_H

#include "../Engine/Engine_Core.h"
#include "../Engine/Buffers/Depth_Pass.h"

RenderPass G_GetRenderPass();
void G_SetRenderPass(RenderPass tex);

extern void G_SetRenderShadowMap(bool b);
extern GLuint G_GetShadowMap();
extern void G_SetShadowMap(GLuint tex);
extern bool G_RenderShadowMap();

extern EntityManager* G_GetManager();
extern uint32_t GetAmountOfEntities();
extern void G_SetManager(EntityManager* mgr);


#endif // !ENGINE_UTILITYFUNCTIONS_H

