#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "GL/glew.h"
#include <string>
#include <vector>
#include "ECS/Component.h"
#include "ECS/Entity.h"

class Texture2D : public Component {

public:

	void OnImGuiRender();


	Texture2D();
	virtual ~Texture2D();

	bool loadTexture(const std::string& fileName, bool generateMipMaps = true);
	bool loadHDRTexture(const std::string& fileName);
	bool loadCubemap(const std::vector<std::string> fileNames);
	void bind(GLuint texUint = 0);
	void unbind(GLuint texUnit = 0);
	GLuint GetTexture();


private:

	int m_textureWidth = 0;
	int m_TextureHeight = 0;

	GLuint m_Texture;
};

#endif // ! TEXTURE2D_H
