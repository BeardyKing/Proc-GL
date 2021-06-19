#ifndef SHADER_BLIN_PHONG_UNIFORMS_H
#define SHADER_BLIN_PHONG_UNIFORMS_H

#include "Shader_UniformSetter.h"
#include "../Engine/ECS_List.h"
#include "../Texture2D.h"
#include <vector>

GLuint GetShadowMap();
bool RenderShadowMap();


namespace uniform {
	class Shader_Blinn_Phong_Uniforms : public Shader_Uniforms
	{
	public:
		Shader_Blinn_Phong_Uniforms();
		~Shader_Blinn_Phong_Uniforms();

		void LoadTextures(Entity& _shader);

		void SetUniformMVP(glm::mat4& model, glm::mat4& view, glm::mat4& projection, ShaderProgram& _shader, Camera& _camera);
		void SetUniformCustom(ShaderProgram& _shader)override;
		void OnImGuiRender()override;

	public: //helperFunction
		void SetBaseColor(glm::vec3 _colour) override;
	private:
		glm::vec3 _baseColor = glm::vec3(1.0f,0.38f,0.38f);
		std::unique_ptr <Texture2D[]> _pbr_textures;
		int numberOfTextures = 0;
		bool castShadows = true;
		bool recieveShadows = true;
		float specular = 0;

	};	
	void Shader_Blinn_Phong_Uniforms::SetBaseColor(glm::vec3 _color) { _baseColor = _color; }

	void Shader_Blinn_Phong_Uniforms::SetUniformMVP(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection, ShaderProgram& _shader, Camera& _camera) {
		//glm::vec3 cameraPosition = GetManager()->FindActiveCamera()->getComponent<Transform>().position; // Also viable
		glm::mat4 _newModel = glm::mat4(1.0f);
		_shader.setUniform("model", model);
		_shader.setUniform("view", view);
		_shader.setUniform("projection", projection);	
		
	
	}
	
	void Shader_Blinn_Phong_Uniforms::SetUniformCustom(ShaderProgram& _shader){

		auto m_lights = GetManager()->FindLights();
		auto camPos = GetManager()->FindActiveCamera()->getComponent<Transform>().position;

		_pbr_textures[0].bind(0);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, GetShadowMap());

		
		_shader.use();
		_shader.setUniformSampler("diffuseTexture",0);	// 0 = albedo

		if (RenderShadowMap() && castShadows){
			_shader.setUniform("view", glm::mat4(1));
			_shader.setUniform("projection", m_lights[0]->getComponent<LightObject>().LightSpaceMatrix());
		}
		if (recieveShadows){
			_shader.setUniform("lightSpaceMatrix", m_lights[0]->getComponent<LightObject>().LightSpaceMatrix());
			_shader.setUniformSampler("shadowMap",1);		// 1 = shadow
			_shader.setUniform("viewPos", camPos);
			_shader.setUniform("lightPos", m_lights[0]->getComponent<Transform>().position);
		}

			_shader.setUniform("_shadowIntensity", m_lights[0]->getComponent<LightObject>().shadowIntensity);
			//_shader.setUniform("_lightColor", m_lights[0]->getComponent<LightObject>().color);
			//_shader.setUniform("_spec", specular);

			/*uniform vec3 _lightColor;
			uniform float _spec;*/
		



		
	}

	void Shader_Blinn_Phong_Uniforms::OnImGuiRender() {
		ImGui::Indent();
		ImGui::Separator();
		if (ImGui::CollapsingHeader("Public Uniforms", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
			ImGui::Indent();

			ImGui::Checkbox("Cast Shadows", &castShadows);
			ImGui::Checkbox("Recieve Shadows", &recieveShadows);
			ImGui::DragFloat("Specular", &specular);

			for (size_t i = 0; i < numberOfTextures; i++) {
				std::string name;
				name = "tex : " + std::to_string(i);
				if (ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
					int adjustedWidth = ImGui::GetContentRegionAvailWidth() * 1024 / 1024;
					ImGui::Image((void*)_pbr_textures[i].GetTexture(), ImVec2(ImGui::GetContentRegionAvailWidth(), adjustedWidth));
				}
			}
			ImGui::Unindent();
		}
		ImGui::Unindent();
	}
	
	void Shader_Blinn_Phong_Uniforms::LoadTextures(Entity& _shader) {
		auto tex = _shader.getComponent<ShaderProgram>().GetTextures();
		numberOfTextures = tex.size();
		_pbr_textures = std::make_unique<Texture2D[]>(numberOfTextures);

		for (size_t i = 0; i < numberOfTextures; i++){
			_pbr_textures[i].loadTexture(tex[i], true);	  
		}
	}

	Shader_Blinn_Phong_Uniforms::Shader_Blinn_Phong_Uniforms() {}
	Shader_Blinn_Phong_Uniforms::~Shader_Blinn_Phong_Uniforms() {}
}

#endif // !SHADER_BLIN_PHONG_UNIFORMS_H
