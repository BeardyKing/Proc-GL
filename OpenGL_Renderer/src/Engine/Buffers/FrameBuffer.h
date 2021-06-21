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

private:

public :
	GLuint FramebufferName;
	GLuint renderedTexture;
	GLuint depthTexture;
};
#endif // !FRAMEBUFFER_H
