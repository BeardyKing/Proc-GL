#ifndef DEPTHBUFFER_H
#define DEPTHBUFFER_H

#include "Texture2D.h"


class DepthBuffer
{
public:
	DepthBuffer();
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
