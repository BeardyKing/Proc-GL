#ifndef SKYBOX_H
#define SKYBOX_H

#include "GL/glew.h"

#include "../ECS/Engine_ECS.h"
#include "../Engine/Texture2D.h"

#include <string>
#include <vector>
#include <iostream>

class SkyBox : public Component {
public:

	virtual void OnImGuiRender() override;

	SkyBox(const std::vector<std::string> fileNames);
	GLuint GetSkyboxTexture();
	~SkyBox();

private:
	Texture2D m_Texture;
};

#endif // ! TEXTURE2D_H
