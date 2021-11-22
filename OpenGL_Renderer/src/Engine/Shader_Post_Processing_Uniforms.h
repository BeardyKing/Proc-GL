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
		void SetInt(const int& value, const std::string& name) override;
		void SetBaseColor(glm::vec3 _colour) override;
	private:
		glm::vec3 _baseColor = glm::vec3(1.0f, 0.38f, 0.38f);
		std::unique_ptr <Texture2D[]> m_pbr_textures;
		std::vector<GLuint>render_texture;
		int numberOfTextures = 0;

		float threshold = 1.0f;
		float strength = 1.0f;
		float resolution = 512.0f;
		float radius = 1.0f;
		glm::vec2 direction = glm::vec2(1);

		int isVertical = 0;

	};

	void Shader_Post_Processing_Uniforms::SetRenderTexture(GLuint fbo_render_texture) {
		render_texture.emplace_back(fbo_render_texture);
	}

	void Shader_Post_Processing_Uniforms::SetBaseColor(glm::vec3 _color) { _baseColor = _color; }

	void Shader_Post_Processing_Uniforms::SetInt(const int& value, const std::string& name) {
		if (name == "isVertical") {
			isVertical = value;
		}
	}

	void Shader_Post_Processing_Uniforms::SetUniformMVP(glm::mat4& model, glm::mat4& view, glm::mat4& projection, ShaderProgram& _shader, Camera& _camera) {
		_shader.setUniform("model", model);
		_shader.setUniform("view", view);
		_shader.setUniform("projection", projection);
	}

	void Shader_Post_Processing_Uniforms::SetUniformCustom(ShaderProgram& _shader){
		
		Entity* cam = G_GetManager()->FindActiveCamera();

		_shader.use();
		
		if (render_texture[0]){
			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_2D, render_texture[0]); 
		}

		if (render_texture[1]) {
			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D, render_texture[1]);
		}

		_shader.setUniform("lightCol", _baseColor);
		_shader.setUniform("textureScale", glm::vec2(1));

		_shader.setUniform("threshold"		, threshold);
		_shader.setUniform("strength"		, strength);
		_shader.setUniform("radius"			, radius);
		if (isVertical == 0){
			direction = glm::vec2(1.0f, 0.0f);
			resolution = cam->getComponent<FPSCamera>().ImGuiWindowSize.x;
			std::cout << "IS VERT FALSE" << std::endl;
		}
		else if( isVertical == 1){
			direction = glm::vec2(0.0f, 1.0f);
			resolution = cam->getComponent<FPSCamera>().ImGuiWindowSize.y;
			std::cout << "IS VERT TRUE" << std::endl;
		}
		_shader.setUniform("resolution"		, resolution);
		_shader.setUniform("direction"		, direction);
		_shader.setUniform("isVertical"		, isVertical);


		

		_shader.setUniformSampler("rendertextureColor", 0);		// 0 = albedo
		_shader.setUniformSampler("rendertextureDepth", 1);		// 0 = albedo
		
		render_texture.clear();
	}

	void Shader_Post_Processing_Uniforms::OnImGuiRender() {
		ImGui::Indent();

		/*ImGui::SameLine();
		ImGui::Selectable("isVertical");
		ImGui::NextColumn();
		ImGui::SliderInt("##isVertical", &isVertical, 0, 1);
		ImGui::NextColumn();*/


		ImGui::SliderFloat("threshold", &threshold, 0.0f, 100.0f);
		ImGui::NextColumn();



		ImGui::SliderFloat("strength", &strength, 0.0f, 100.0f);
		ImGui::NextColumn();

		ImGui::SliderFloat("radius", &radius, -10.0f, 10.0f);
		ImGui::NextColumn();


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
