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
	unsigned char* imageData = stbi_load(fileName.c_str(), &width, &height, &components, 0);
	if (imageData){

		glGenTextures(1, &m_Texture);
		

		GLenum format;
		if (components == 1)
			format = GL_RED;
		else if (components == 3)
			format = GL_RGB;
		else if (components == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if (generateMipMaps){
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
		else{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(imageData);
	}
	else{
		std::cout << "Texture failed to load at path: " << fileName << std::endl;
		stbi_image_free(imageData);
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, 0); // unbind

	return true;
}

void Texture2D::bind(GLuint texUnit) {
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
}

void Texture2D::unbind(GLuint texUnit) {
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, 0);
}


//int widthInBytes = width * 4; 
//unsigned char* top = NULL;
//unsigned char* bottom = NULL;
//unsigned char temp = 0;
//int halfHeight = height / 2;
//
//for (int row = 0; row < halfHeight; row++)
//{
//	top = imageData + row * widthInBytes;
//	bottom = imageData + (height - row - 1) * widthInBytes;
//
//	for (int col = 0; col < widthInBytes; col++)
//	{
//		temp = *top;
//		*top = *bottom;
//		*bottom = temp;
//		top++;
//		bottom++;
//	}
//}

