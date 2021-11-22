#ifndef SHADER_POST_PROCESSING_UNIFORMS_H
#define SHADER_POST_PROCESSING_UNIFORMS_H

#include <GL/glew.h>
#include "Shader_UniformSetter.h"
#include "../Engine/ECS_List.h"

namespace uniform {
	class Shader_Post_Processing_Uniforms : public Shader_Uniforms
	{
	public:
		Shader_Post_Processing_Uniforms();
		~Shader_Post_Processing_Uniforms();
		void SetUniformMVP(glm::mat4& model, glm::mat4& view, glm::mat4& projection, ShaderProgram& _shader, Camera& _camera);
		void SetUniformCustom(ShaderProgram& _shader)override;
		void OnImGuiRender()override;

		void LoadTextures(Entity& _shader);
		void SetRenderTexture(GLuint fbo_render_texture);

	public: //helperFunction
		void SetBaseColor(glm::vec3 _colour) override;
	private:
		glm::vec3 _baseColor = glm::vec3(1.0f,0.38f,0.38f);
		std::unique_ptr <Texture2D[]> m_pbr_textures;
		std::vector<GLuint>render_texture;
		int numberOfTextures = 0;
		

	};

	void Shader_Post_Processing_Uniforms::SetRenderTexture(GLuint fbo_render_texture) {
		render_texture.emplace_back(fbo_render_texture);
	}

	void Shader_Post_Processing_Uniforms::SetBaseColor(glm::vec3 _color) { _baseColor = _color; }


	void Shader_Post_Processing_Uniforms::SetUniformMVP(glm::mat4& model, glm::mat4& view, glm::mat4& projection, ShaderProgram& _shader, Camera& _camera) {
		_shader.setUniform("model", model);
		_shader.setUniform("view", view);
		_shader.setUniform("projection", projection);
	}

	void Shader_Post_Processing_Uniforms::SetUniformCustom(ShaderProgram& _shader){
		_shader.use();
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, render_texture[0]); 

		_shader.setUniform("lightCol", _baseColor);
		_shader.setUniform("textureScale", glm::vec2(1));

		_shader.setUniformSampler("screenTexture", 0);		// 0 = albedo

	}

	void Shader_Post_Processing_Uniforms::OnImGuiRender() {
		ImGui::Indent();
		if (ImGui::CollapsingHeader("public Uniforms", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
			ImGui::ColorPicker3("Base Color Picker", &_baseColor.r);
		}
		ImGui::Unindent();
	}

	void Shader_Post_Processing_Uniforms::LoadTextures(Entity& _shader) {
		auto tex = _shader.getComponent<ShaderProgram>().GetTextures();
		numberOfTextures = tex.size();
		m_pbr_textures = std::make_unique<Texture2D[]>(numberOfTextures);

		for (int i = 0; i < numberOfTextures; i++) {
			m_pbr_textures[i].LoadTexture(tex[i], true);
		}
	}

	Shader_Post_Processing_Uniforms::Shader_Post_Processing_Uniforms(){}
	Shader_Post_Processing_Uniforms::~Shader_Post_Processing_Uniforms(){}
}

#endif // !SHADER_DEFAULTUNIFORMS_H
