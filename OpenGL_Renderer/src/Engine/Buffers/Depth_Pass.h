#ifndef DEPTH_PASS_H
#define DEPTH_PASS_H

#include <GL/glew.h>

class RenderPass
{
public:
	RenderPass();
	~RenderPass();

	void Bind();
	void UnBind();
	void GenerateFrameBuffer(float x, float y);
	void UpdateFrameBufferTextureSize(float x, float y);
	void DeleteFrameBuffer();

	GLuint GetFrameBuffer();
	GLuint GetRenderBuffer();
	GLuint GetDepthBuffer();
private:
	GLuint m_frameBuffer;
	GLuint m_depthBuffer;
	GLuint m_renderBuffer;

};
#endif // !FRAMEBUFFER_H
