#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

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
