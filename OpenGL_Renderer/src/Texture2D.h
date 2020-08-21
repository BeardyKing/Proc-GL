#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "GL/glew.h"
#include <string>


class Texture2D {

public:
	Texture2D();
	virtual ~Texture2D();

	bool loadTexture(const std::string& fileName, bool generateMipMaps = true);
	void bind(GLuint texUint = 0);
private:
	GLuint m_Texture;
};


#endif // ! TEXTURE2D_H
