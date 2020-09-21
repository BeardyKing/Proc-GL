#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "GL/glew.h"
#include <string>
#include <vector>



class Texture2D {

public:
	Texture2D();
	virtual ~Texture2D();

	bool loadTexture(const std::string& fileName, bool generateMipMaps = true);
	bool loadHDRTexture(const std::string& fileName);
	bool loadCubemap(const std::vector<std::string> fileNames);
	void bind(GLuint texUint = 0);
	void unbind(GLuint texUnit = 0);
private:
	GLuint m_Texture;
};


#endif // ! TEXTURE2D_H
