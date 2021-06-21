#ifndef SHADER_PBR_UNIFORMS_H
#define SHADER_PBR_UNIFORMS_H

#include "../Engine/ECS_List.h"
#include "../Engine/Engine_Core.h"
#include <vector>

GLuint GetShadowMap();
bool RenderShadowMap();

namespace uniform {
	class Shader_PBR_Uniforms : public Shader_Uniforms
	{
	public:
		Shader_PBR_Uniforms();
		~Shader_PBR_Uniforms();

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
	};

	void Shader_PBR_Uniforms::SetBaseColor(glm::vec3 _color) { _baseColor = _color; }

	void Shader_PBR_Uniforms::SetUniformMVP(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection, ShaderProgram& _shader, Camera& _camera) {
		//glm::vec3 cameraPosition = GetManager()->FindActiveCamera()->getComponent<Transform>().position; // Also viable
		_shader.setUniform("model", model);
		_shader.setUniform("view", view);
		_shader.setUniform("projection", projection);
		_shader.setUniform("camPos", _camera.GetPosition());
	}

	void Shader_PBR_Uniforms::SetUniformCustom(ShaderProgram& _shader){
		auto camPos = GetManager()->FindActiveCamera()->getComponent<Transform>().position;

		_pbr_textures[0].Bind(0);
		_pbr_textures[1].Bind(1);
		_pbr_textures[2].Bind(2);
		_pbr_textures[3].Bind(3);
		_pbr_textures[4].Bind(4);

		_shader.setUniformSampler("albedoMap",		0);		// 0 = albedo
		_shader.setUniformSampler("normalMap",		1);		// 1 = normal
		_shader.setUniformSampler("metallicMap",	2);		// 2 = metalic
		_shader.setUniformSampler("roughnessMap",	3);		// 3 = roughness
		_shader.setUniformSampler("aoMap",			4);		// 4 = ambient 


		auto m_lights = GetManager()->FindLights();

		_shader.use();
		_shader.setUniform("amountOfLights", (GLint)m_lights.size());

		for (unsigned int i = 0; i < m_lights.size(); ++i) {
			std::string str1 = "lightPositions[" + std::to_string(i) + "]";
			_shader.setUniform(str1.c_str(), m_lights[i]->getComponent<Transform>().position);	// set shader uniform for lightPosition[i]

			std::string str2 = "lightColors[" + std::to_string(i) + "]";
			_shader.setUniform(str2.c_str(), m_lights[i]->getComponent<LightObject>().color);		// set shader uniform for lightColors[i]
		}


		glActiveTexture(GL_TEXTURE0 + 5);
		glBindTexture(GL_TEXTURE_2D, GetShadowMap());


		_shader.use();

		if (RenderShadowMap() && castShadows) {
			_shader.setUniform("view", glm::mat4(1));
			_shader.setUniform("projection", m_lights[0]->getComponent<LightObject>().LightSpaceMatrix());
		}
		if (recieveShadows) {
			_shader.setUniform("lightSpaceMatrix", m_lights[0]->getComponent<LightObject>().LightSpaceMatrix());
		}

		_shader.setUniformSampler("diffuseTexture", 0);	// 0 = albedo
		_shader.setUniformSampler("shadowMap", 1);		// 1 = shadow
		_shader.setUniform("viewPos", camPos);
		_shader.setUniform("lightPos", m_lights[0]->getComponent<Transform>().position);


	}

	void Shader_PBR_Uniforms::OnImGuiRender() {
		ImGui::Indent();
		ImGui::Separator();
		if (ImGui::CollapsingHeader("Public Uniforms",ImGuiTreeNodeFlags_AllowItemOverlap)) {
			ImGui::Indent();
			ImGui::Checkbox("cast Shadows", &castShadows);
			ImGui::Checkbox("recieve Shadows", &recieveShadows);

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
	
	void Shader_PBR_Uniforms::LoadTextures(Entity& _shader) {
		auto tex = _shader.getComponent<ShaderProgram>().GetTextures();
		numberOfTextures = tex.size();
		_pbr_textures = std::make_unique<Texture2D[]>(numberOfTextures);

		for (size_t i = 0; i < numberOfTextures; i++){
			_pbr_textures[i].LoadTexture(tex[i], true);	  
		}
	}

	Shader_PBR_Uniforms::Shader_PBR_Uniforms() {}
	Shader_PBR_Uniforms::~Shader_PBR_Uniforms() {}
}

#endif // !SHADER_PBR_UNIFORMS_H
