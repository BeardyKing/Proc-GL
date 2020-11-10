#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Texture2D.h"


class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void Bind();
	void UnBind();
	void GenerateFrameBuffer();
	void UpdateFrameBufferTextureSize(float x, float y);

private:

public :
	GLuint FramebufferName;
	GLuint renderedTexture;

};



#endif // !FRAMEBUFFER_H
