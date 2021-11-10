#include "Texture2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"


void Texture2D::OnImGuiRender(){
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

	ImGui::Begin("Inspector"); {
		ImGui::Separator();
		if (ImGui::CollapsingHeader("Texture2D", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)){
			ImGui::Indent();

			int adjustedWidth = ImGui::GetContentRegionAvailWidth() * m_textureHeight / m_textureWidth;
			ImGui::Image((void*)m_texture, ImVec2(ImGui::GetContentRegionAvailWidth(), adjustedWidth));

			ImGui::Unindent();
		}
	}
	ImGui::End();
}

Texture2D::~Texture2D() {
	glDeleteTextures(sizeof(m_texture), &m_texture);
}

GLuint Texture2D::GetTexture() {
	return m_texture;
}

glm::vec2 Texture2D::GetTextureSize() {
	return glm::vec2(m_textureWidth, m_textureHeight);
}


bool Texture2D::LoadTexture(const std::string& fileName, bool generateMipMaps) {
	unsigned char* imageData;
	int width, height, components;
	stbi_set_flip_vertically_on_load(true);
	imageData = stbi_load(fileName.c_str(), &width, &height, &components, 0);
	if (imageData){

		glGenTextures(1, &m_texture);

		GLenum format;
		if (components == 1)
			format = GL_RED;
		else if (components == 3)
			format = GL_RGB;
		else if (components == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


		if (m_anisotropicFilteringEnabled){
			if (m_useAnisotropicMaxAmount){
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &m_anisotropicMaxAmount);
			}
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_anisotropicMaxAmount);
		}

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
		m_textureHeight = height;

		stbi_image_free(imageData);
	}
	else{
		std::cout << "Texture failed to load at path: " << fileName << std::endl;
		GenerateFallbackTexture();
		stbi_image_free(imageData);
		std::cout << "WARNING: LOADED FALLBACK TEXTURE" << fileName << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

void Texture2D::GenerateFallbackTexture() {
	glGenTextures(1, &m_texture);

	GLenum format;
	format = GL_RGBA;
	GLubyte texData[] = { 255, 255, 255, 255 };			// Generate white single pixel texture with full alpha
	unsigned char* imageData = (unsigned char*)texData;

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, format, 1, 1, 0, format, GL_UNSIGNED_BYTE, imageData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	std::cout << "generated fallback texture: " << std::endl;
}

bool Texture2D::LoadHDRTexture(const std::string& fileName) {
	stbi_set_flip_vertically_on_load(true);

	int width, height, components;
	float* imageData = stbi_loadf(fileName.c_str(), &width, &height, &components, 0);

	if (imageData){
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
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
	unsigned int envCubemap;
	glGenTextures(1, &envCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return false;
}

bool Texture2D::LoadCubemap(const std::vector<std::string> fileNames) {

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
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

void Texture2D::Bind(GLuint texUnit) {
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture2D::Unbind(GLuint texUnit) {
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, 0);
}