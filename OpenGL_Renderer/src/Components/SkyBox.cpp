#include "../Engine/ECS_List.h"


SkyBox::SkyBox(const std::vector<std::string> fileNames){
	m_Texture.LoadCubemap(fileNames);
}

GLuint SkyBox::GetSkyboxTexture(){
	return m_Texture.GetTexture();
}

SkyBox::~SkyBox() {
}

void SkyBox::OnImGuiRender(){}
