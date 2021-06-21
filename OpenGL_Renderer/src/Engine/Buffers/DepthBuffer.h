#ifndef DEPTHBUFFER_H
#define DEPTHBUFFER_H

#include <GL/glew.h>

class DepthBuffer{
public:
	DepthBuffer(GLuint texture_width, GLuint texture_height);
	~DepthBuffer();

	void Bind();
	void UnBind();
	void GenerateDepthBuffer(GLuint texture_width, GLuint texture_height);
	void DeleteDepthBuffer();

	GLuint GetDepthBuffer();
	GLuint GetFrameBuffer();

private:
	void ResetViewport();

private:
	GLuint m_textureWidth	= 0;
	GLuint m_textureHeight	= 0;
	GLuint m_frameBuffer	= 0;
	GLuint m_depthBuffer	= 0;
};

#endif // !DEPTHBUFFER_H
