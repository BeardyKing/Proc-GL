#include <iostream>
#include "Depth_Pass.h"


RenderPass::RenderPass() :
	m_frameBuffer(0),
	m_renderBuffer(0),
	m_depthBuffer(0)
{

}

RenderPass::~RenderPass() {
	glDeleteFramebuffers(0, &m_frameBuffer); // causes crash on "test" change
}

void RenderPass::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
}

void RenderPass::UnBind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint RenderPass::GetFrameBuffer() {
	return m_frameBuffer;
}

GLuint RenderPass::GetDepthBuffer() {
	return m_depthBuffer;
}

GLuint RenderPass::GetRenderBuffer() {
	return m_renderBuffer;
}

void RenderPass::DeleteFrameBuffer() {
	glDeleteBuffers(GL_FRAMEBUFFER, &m_frameBuffer);
}

void RenderPass::UpdateFrameBufferTextureSize(float screen_width, float screen_height) {
	glBindTexture(GL_TEXTURE_2D, m_renderBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_width, screen_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	glBindTexture(GL_TEXTURE_2D, m_depthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, screen_width, screen_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glViewport(0, 0, screen_width, screen_height);
}

void RenderPass::GenerateFrameBuffer(float screen_width, float screen_height) {
	m_frameBuffer = 0;
	glGenFramebuffers(1, &m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	glGenTextures(1, &m_renderBuffer);
	glBindTexture(GL_TEXTURE_2D, m_renderBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_width, screen_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);

	glGenTextures(1, &m_depthBuffer);
	glBindTexture(GL_TEXTURE_2D, m_depthBuffer);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, screen_width, screen_width, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_renderBuffer, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthBuffer, 0);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	glBindBuffer(GL_FRAMEBUFFER, 0);
}