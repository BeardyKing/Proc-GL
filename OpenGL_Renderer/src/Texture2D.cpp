#include "Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include <iostream>

Texture2D::Texture2D() : m_Texture(0){}

void Texture2D::OnImGuiRender(){
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

	ImGui::Begin("Inspector"); {
		ImGui::Separator();
		if (ImGui::CollapsingHeader("Texture2D", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)){
			ImGui::Indent();

			int adjustedWidth = ImGui::GetContentRegionAvailWidth() * m_TextureHeight / m_textureWidth;
			ImGui::Image((void*)m_Texture, ImVec2(ImGui::GetContentRegionAvailWidth(), adjustedWidth));

			ImGui::Unindent();
		}
	}
	ImGui::End();
}

Texture2D::~Texture2D() {
	glDeleteTextures(sizeof(m_Texture), &m_Texture);
}

GLuint Texture2D::GetTexture() {
	return m_Texture;
}

glm::vec2 Texture2D::GetTextureSize() {
	return glm::vec2(m_textureWidth, m_TextureHeight);
}


bool Texture2D::loadTexture(const std::string& fileName, bool generateMipMaps) {
	unsigned char* imageData;
	int width, height, components;
	stbi_set_flip_vertically_on_load(true);
	imageData = stbi_load(fileName.c_str(), &width, &height, &components, 0);
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

		std::cout << "Texture loaded at path: " << fileName << std::endl;

		m_textureWidth = width;
		m_TextureHeight = height;

		stbi_image_free(imageData);
	}
	else{
		std::cout << "Texture failed to load at path: " << fileName << std::endl;
		stbi_image_free(imageData);
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

bool Texture2D::loadHDRTexture(const std::string& fileName) {
	stbi_set_flip_vertically_on_load(true);
	int width, height, components;
	float* imageData = stbi_loadf(fileName.c_str(), &width, &height, &components, 0);
	if (imageData){
		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, imageData);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(imageData);
		return true;
	}
	else{
		std::cout << "Failed to load HDR image at path: " << fileName << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	return false;
}

bool Texture2D::loadCubemap(const std::vector<std::string> fileNames) {
	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);
	stbi_set_flip_vertically_on_load(true);
	int width, height, components;
	for (unsigned int i = 0; i < fileNames.size(); i++){
		unsigned char* imageData = stbi_load(fileNames[i].c_str(), &width, &height, &components, 0);
		if (imageData){
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
			stbi_image_free(imageData);
		}
		else{
			std::cout << "Cubemap tex failed to load at path: " << fileNames[i] << std::endl;
			stbi_image_free(imageData);
			return false;
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);

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

