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
		void SetFloat(const float& value, const std::string& name) override;
		void SetBaseColor(glm::vec3 _colour) override;
	private:
		glm::vec3 _baseColor = glm::vec3(1.0f, 0.38f, 0.38f);
		std::unique_ptr <Texture2D[]> m_pbr_textures;
		std::vector<GLuint>render_texture;
		int numberOfTextures = 0;

		float threshold = 1.0f;
		float strength = 10.0f;
		float resolution = 512.0f;
		glm::vec2 screen_resolution = glm::vec2(1);
		float radius = 0.011f;
		glm::vec2 direction = glm::vec2(1);

		int isVertical = 0;
		int renderPassCount = 0;

		float blurStrength_0 = 1;
		float blurStrength_1 = 1;
		float blurStrength_2 = 1;

		float vignette_pow_intensity = 0.059;
		float vignette_multiply_intensity = 50;

		bool isColorCorrection = 0;
		bool isVignette = 0;
		bool isDepthOfField = 0;
		bool isFlipImage = 0;
		
		int blendPass = 0;
		float bloomStrength = 1;
	};

	void Shader_Post_Processing_Uniforms::SetRenderTexture(GLuint fbo_render_texture) {
		render_texture.emplace_back(fbo_render_texture);
	}

	void Shader_Post_Processing_Uniforms::SetBaseColor(glm::vec3 _color) { _baseColor = _color; }

	void Shader_Post_Processing_Uniforms::SetInt(const int& value, const std::string& name) {
		if (name == "isVertical") {
			isVertical = value;
		}
		if (name == "renderPassCount") {
			renderPassCount = value;
		}
		if (name == "blendPass") {
			blendPass = value;
		}
		if (name == "isColorCorrection") {
			isColorCorrection = value;
		}
		if (name == "isVignette") {
			isVignette = value;
		}
		if (name == "isDepthOfField") {
			isDepthOfField = value;
		}
		if (name == "isFlipImage") {
			isFlipImage = value;
		}
		
	}

	void Shader_Post_Processing_Uniforms::SetFloat(const float& value, const std::string& name) {
		if (name == "blurStrength_0") {
			blurStrength_0 = value;
		}
		if (name == "blurStrength_1") {
			blurStrength_1 = value;
		}
		if (name == "blurStrength_2") {
			blurStrength_2 = value;
		}if (name == "vignette_pow_intensity") {
			vignette_pow_intensity = value;
		}
		if (name == "vignette_multiply_intensity") {
			vignette_multiply_intensity = value;
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
		if (render_texture[2]) {
			glActiveTexture(GL_TEXTURE0 + 2);
			glBindTexture(GL_TEXTURE_2D, render_texture[2]);
		}

		if (render_texture[3]) {
			glActiveTexture(GL_TEXTURE0 + 3);
			glBindTexture(GL_TEXTURE_2D, render_texture[3]);
		}

		_shader.setUniform("lightCol", _baseColor);
		_shader.setUniform("textureScale", glm::vec2(1));

		_shader.setUniform("threshold"		, threshold);
		_shader.setUniform("radius"		, radius);
		_shader.setUniform("strength"		, strength);
		_shader.setUniform("blurStrength_0"			, blurStrength_0);
		_shader.setUniform("blurStrength_1"			, blurStrength_1);
		_shader.setUniform("blurStrength_2"			, blurStrength_2);
		if (isVertical == 0){
			direction = glm::vec2(1.0f, 0.0f);
			resolution = cam->getComponent<FPSCamera>().ImGuiWindowSize.x;
		}
		else if( isVertical == 1){
			direction = glm::vec2(0.0f, 1.0f);
			resolution = cam->getComponent<FPSCamera>().ImGuiWindowSize.y;
		}
		_shader.setUniform("resolution"		, resolution);
		screen_resolution = glm::vec2(cam->getComponent<FPSCamera>().ImGuiWindowSize.x, cam->getComponent<FPSCamera>().ImGuiWindowSize.y);
		_shader.setUniform("screen_resolution"	, screen_resolution);
		_shader.setUniform("direction"		, direction);
		_shader.setUniform("isVertical"		, isVertical);

		_shader.setUniform("isColorCorrection"	, isColorCorrection);
		_shader.setUniform("isVignette"			, isVignette);
		_shader.setUniform("isDepthOfField"		, isDepthOfField);
		_shader.setUniform("isFlipImage"		, isFlipImage);
		
		_shader.setUniform("renderPassCount", renderPassCount);
		_shader.setUniform("blendPass"		, blendPass);

		_shader.setUniform("vignette_pow_intensity"		, vignette_pow_intensity);
		_shader.setUniform("vignette_multiply_intensity", vignette_multiply_intensity);


		_shader.setUniformSampler("rendertextureColor", 0);		// 0 = albedo
		_shader.setUniformSampler("rendertextureDepth", 1);		// 0 = albedo
		_shader.setUniformSampler("rendertextureFinalColour", 2);		// 0 = albedo
		_shader.setUniformSampler("rendertextureFinalBlur", 3);		// 0 = albedo
		
		render_texture.clear();
	}

	void Shader_Post_Processing_Uniforms::OnImGuiRender() {
		ImGui::Indent();

		/*ImGui::SameLine();
		ImGui::Selectable("isVertical");
		ImGui::NextColumn();
		ImGui::SliderInt("##isVertical", &isVertical, 0, 1);
		ImGui::NextColumn();*/

		// VIGNETTE
		
		ImGui::SliderFloat("vignette_pow_intensity", &vignette_pow_intensity, 0.0f, 1.0f);
		ImGui::NextColumn(); 
		
		ImGui::SliderFloat("vignette_multiply_intensity", &vignette_multiply_intensity, 0.0f, 50.0f);
		ImGui::NextColumn();


		ImGui::SliderFloat("threshold", &threshold, 0.0f, 100.0f);
		ImGui::NextColumn(); 
		
		ImGui::SliderFloat("bloomStrength", &bloomStrength, -1.0f, 3.0f);
		ImGui::NextColumn();
		ImGui::SliderFloat("strength", &strength, 0.0f, 15.0f);
		ImGui::NextColumn();

		ImGui::SliderFloat("radius", &radius, 0.0f, 0.1f);
		ImGui::NextColumn(); 

		ImGui::SliderFloat("blurStrength_0", &blurStrength_0, 0.0f, 10.0f);
		ImGui::NextColumn(); 
		ImGui::SliderFloat("blurStrength_1", &blurStrength_1, 0.0f, 10.0f);
		ImGui::NextColumn(); 
		ImGui::SliderFloat("blurStrength_2", &blurStrength_2, 0.0f, 10.0f);
		ImGui::NextColumn();

		ImGui::SliderInt("renderPassCount", &renderPassCount, -10.0f, 10.0f);
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
