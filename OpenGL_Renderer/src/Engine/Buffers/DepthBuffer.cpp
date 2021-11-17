#include <iostream>
#include "DepthBuffer.h"
#include "../../Engine/ECS_List.h"
#include "../../Engine/Engine_UtilityFunctions.h"

DepthBuffer::DepthBuffer(GLuint texture_width, GLuint texture_height) :
    m_textureWidth(texture_width), 
    m_textureHeight(texture_height), 
    m_frameBuffer(0), 
    m_depthBuffer(0)
{
    GenerateDepthBuffer(texture_width, texture_height);
}

DepthBuffer::~DepthBuffer() {
    //DeleteDepthBuffer(); // causes crash on "test" change
}

void DepthBuffer::DeleteDepthBuffer() {
    glDeleteBuffers(GL_FRAMEBUFFER, &m_frameBuffer);
}

GLuint DepthBuffer::GetDepthBuffer() {
    return m_depthBuffer;
}

GLuint DepthBuffer::GetFrameBuffer() {
    return m_frameBuffer;
}

void DepthBuffer::Bind() {
  
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    G_SetRenderShadowMap(true);

    glViewport(0, 0, m_textureWidth, m_textureHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    glClear(GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glCullFace(GL_NONE);
}

void DepthBuffer::UnBind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ResetViewport();
}

void DepthBuffer::ResetViewport() {
    glCullFace(GL_BACK);
    auto& cam = G_GetManager()->FindActiveCamera()->getComponent<FPSCamera>();
    glViewport(0, 0, static_cast<int>(cam.ImGuiWindowSize.x), static_cast<int>(cam.ImGuiWindowSize.y));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    G_SetRenderShadowMap(false);
}


void DepthBuffer::GenerateDepthBuffer(GLuint texture_width, GLuint texture_height) {
    m_textureWidth = texture_width;
    m_textureHeight = texture_width;

    glGenFramebuffers(1, &m_frameBuffer);
    // create depth texture
    glGenTextures(1, &m_depthBuffer);
    glBindTexture(GL_TEXTURE_2D, m_depthBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_textureWidth, m_textureHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthBuffer, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}