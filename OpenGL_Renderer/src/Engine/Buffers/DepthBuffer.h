#ifndef DEPTHBUFFER_H
#define DEPTHBUFFER_H

//#define GLEW_STATIC
#include <GL/glew.h>

class DepthBuffer
{
public:
	DepthBuffer();
	DepthBuffer(int x, int y);
	~DepthBuffer();

	void Bind();
	void UnBind();
	void GenerateDepthBuffer(float x, float y);
	void UpdateDepthBufferTextureSize(float x, float y);

private:

public :
	unsigned int SHADOW_WIDTH, SHADOW_HEIGHT;
	unsigned int depthMapFBO;
	unsigned int depthMap;
};



#endif // !DEPTHBUFFER_H
