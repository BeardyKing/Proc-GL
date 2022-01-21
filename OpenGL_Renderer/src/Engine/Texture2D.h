#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "GL/glew.h"

#include "../ECS/Engine_ECS.h"

#include <string>
#include <vector>
#include <iostream>

class Texture2D : public Component {
public:

	void OnImGuiRender();

	virtual ~Texture2D();

	bool LoadTexture(const std::string& fileName, bool generateMipMaps = true);
	bool LoadHDRTexture(const std::string& fileName);
	bool LoadCubemap(const std::vector<std::string> fileNames);
	void GenerateFallbackTexture();

	void Bind(GLuint texUint = 0);
	void Unbind(GLuint texUnit = 0);

	GLuint GetTexture();
	glm::vec2 GetTextureSize();

private:
	std::string debug_filename = "";
	bool isUsingCubeMap = false;
	int m_textureWidth = 0;
	int m_textureHeight = 0;

	bool m_anisotropicFilteringEnabled = true;
	bool m_useAnisotropicMaxAmount = false;
	GLfloat m_anisotropicMaxAmount = 1;

	GLuint m_texture;
};

#endif // ! TEXTURE2D_H
