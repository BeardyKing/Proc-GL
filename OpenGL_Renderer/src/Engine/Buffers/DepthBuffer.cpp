#include <iostream>
#include "DepthBuffer.h"
#include "../../Engine/ECS_List.h"


void SetRenderShadowMap(bool b);
EntityManager* GetManager();
GLuint GetShadowMap();
void SetShadowMap(GLuint tex);

DepthBuffer::DepthBuffer() :
    SHADOW_WIDTH(0), SHADOW_HEIGHT(0),depthMapFBO(0),depthMap(0)
{
    
}

DepthBuffer::DepthBuffer(int x, int y) :
    SHADOW_WIDTH(x), SHADOW_HEIGHT(y), depthMapFBO(0), depthMap(0)
{
    GenerateDepthBuffer(x, y);
}

DepthBuffer::~DepthBuffer() {

}

void DepthBuffer::Bind() {
  

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SetRenderShadowMap(true);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glCullFace(GL_NONE);
}

void DepthBuffer::UnBind() {
    glCullFace(GL_BACK);
    //SetShadowMap(depthMap);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    auto& cam = GetManager()->FindActiveCamera()->getComponent<FPSCamera>();

    // reset viewport
    glViewport(0, 0, cam.ImGuiWindowSize.x, cam.ImGuiWindowSize.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SetRenderShadowMap(false);
}

//void DepthBuffer::UpdateDepthBufferTextureSize(float x, float y) {
//    glBindTexture(GL_TEXTURE_2D, renderedTexture);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
//
//	glBindTexture(GL_TEXTURE_2D, depthTexture);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, x, y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
//	
//    glViewport(0, 0, x, y);
//}


void DepthBuffer::GenerateDepthBuffer(float x, float y) {
        SHADOW_WIDTH = 1024 * 4;
        SHADOW_HEIGHT = 1024 * 4;

        glGenFramebuffers(1, &depthMapFBO);
        // create depth texture
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        // attach depth texture as FBO's depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
}