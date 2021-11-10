#ifndef SHADER_STANDARD_SKYBOX_UNIFORMS_H
#define SHADER_STANDARD_SKYBOX_UNIFORMS_H


#include "Engine_Core.h"
#include "Engine_UtilityFunctions.h"

#include <vector>

namespace uniform {
	class Shader_Standard_Skybox_Uniforms : public Shader_Uniforms
	{
	public:
		Shader_Standard_Skybox_Uniforms();
		~Shader_Standard_Skybox_Uniforms();

		void LoadTextures(Entity& _shader);

		void SetUniformMVP(glm::mat4& model, glm::mat4& view, glm::mat4& projection, ShaderProgram& _shader, Camera& _camera);
		void SetUniformCustom(ShaderProgram& _shader)override;
		void OnImGuiRender()override;

	public: //helperFunction
		/*void SetBaseColor(glm::vec3 _colour) override;
		void SetTextureScale(glm::vec2 _scale) override;
		void SetColour(const glm::vec4& value, const std::string& name) override;
		void SetFloat(const float& value, const std::string& name) override;
		void SetInt(const int& value, const std::string& name) override;*/

	private:
		Texture2D skybox;
		int numberOfTextures = 0;
	};

	void Shader_Standard_Skybox_Uniforms::SetUniformMVP(glm::mat4& model, glm::mat4& view, glm::mat4& projection, ShaderProgram& shader, Camera& _camera) {
		//glm::vec3 cameraPosition = G_GetManager()->FindActiveCamera()->getComponent<Transform>().position; // Also viable
		glm::mat4 _newModel = glm::mat4(1.0f);
		shader.setUniform("modelMatrix", model);
		shader.setUniform("viewMatrix", view);
		shader.setUniform("projMatrix", projection);
		shader.setUniform("camPos", _camera.GetPosition());
	}

	void Shader_Standard_Skybox_Uniforms::SetUniformCustom(ShaderProgram& shader) {


		auto camPos = G_GetManager()->FindActiveCamera()->getComponent<Transform>().position;

		shader.use();
		
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.GetTexture()); // instead of _shader.bind

	}

	void Shader_Standard_Skybox_Uniforms::OnImGuiRender() {}

	void Shader_Standard_Skybox_Uniforms::LoadTextures(Entity& _shader) {
		
		auto tex = _shader.getComponent<ShaderProgram>().GetTextures();
		numberOfTextures = tex.size();

		if (tex.size() < 6){
			std::cout << "Err: @ Shader_Standard_Skybox_Uniforms : Not enough images to generate cube map" << std::endl;
			return;
		}

		skybox.LoadCubemap(tex);
		std::cout << "Loaded cubemap" << std::endl;

	}

	Shader_Standard_Skybox_Uniforms::Shader_Standard_Skybox_Uniforms() {}
	Shader_Standard_Skybox_Uniforms::~Shader_Standard_Skybox_Uniforms() {}
}

#endif // !SHADER_STANDARD_LIT_UNIFORMS_H
