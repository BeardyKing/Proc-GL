#include "Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include <iostream>

Texture2D::Texture2D() 
: m_Texture(0)
{

}

Texture2D::~Texture2D() {

}

bool Texture2D::loadTexture(const std::string& fileName, bool generateMipMaps) {
	int width, height, components;

	unsigned char* imageData = stbi_load(fileName.c_str(), &width, &height, &components, STBI_rgb_alpha);
	if (imageData == NULL){
		std::cerr << "Error loading image texture ' " << fileName << " '" << std::endl;
		return false;
	}

	//invert image

	int widthInBytes = width * 4;
	unsigned char* top = NULL;
	unsigned char* bottom = NULL;
	unsigned char temp = 0;
	int halfHeight = height / 2;

	for (int row = 0; row < halfHeight; row++)
	{
		top = imageData + row * widthInBytes;
		bottom = imageData + (height - row - 1) * widthInBytes;

		for (int col = 0; col < widthInBytes; col++)
		{ 
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}



	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	 // "x" axsis U
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	 // "y" axsis V
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR); // "bilinair" for scaling down
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR); // "bilinair" for scaling up


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,0, GL_RGBA, GL_UNSIGNED_BYTE, imageData); // 0 = FULL RES LOD

	if (generateMipMaps){
		//glGenerateMipmap(GL_TEXTURE_2D); // MIP MAPS CRASHING OPEN GL NOT SURE WHY
	}

	stbi_image_free(imageData);
	glBindTexture(GL_TEXTURE_2D, 0); // unbind

	return true;
}

void Texture2D::bind(GLuint texUint) {
	glBindTexture(GL_TEXTURE_2D, m_Texture);

}