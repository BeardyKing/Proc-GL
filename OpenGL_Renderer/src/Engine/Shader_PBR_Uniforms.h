#ifndef SHADER_PBR_UNIFORMS_H
#define SHADER_PBR_UNIFORMS_H

#include "Shader_UniformSetter.h"
#include "../Engine/ECS_List.h"
#include "../Texture2D.h"
#include <vector>


namespace uniform {
	class Shader_PBR_Uniforms : public Shader_Uniforms
	{
	public:
		Shader_PBR_Uniforms();
		~Shader_PBR_Uniforms();

		void SetUniformMVP(glm::mat4& model, glm::mat4& view, glm::mat4& projection, ShaderProgram& _shader, Camera& _camera);
		void SetUniformCustom(ShaderProgram& _shader)override;
		void OnImGuiRender()override;

	public: //helperFunction
		void SetBaseColor(glm::vec3 _colour) override;
	private:
		glm::vec3 _baseColor = glm::vec3(1.0f,0.38f,0.38f);
		std::unique_ptr <Texture2D[]> _pbr_textures;
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
		_pbr_textures[0].bind(0);
		_pbr_textures[1].bind(1);
		_pbr_textures[2].bind(2);
		_pbr_textures[3].bind(3);
		_pbr_textures[4].bind(4);

		_shader.setUniformSampler("albedoMap", 0);
		_shader.setUniformSampler("normalMap", 1);
		_shader.setUniformSampler("metallicMap", 2);
		_shader.setUniformSampler("roughnessMap", 3);
		_shader.setUniformSampler("aoMap", 4);


		

		//m_pbr_cube->getComponent<ShaderProgram>().use();
		//m_pbr_cube->getComponent<ShaderProgram>().setUniform("amountOfLights", m_amountOfLights);

		//for (unsigned int i = 0; i < m_amountOfLights; ++i) {
		//	std::string str1 = "lightPositions[" + std::to_string(i) + "]";
		//	m_pbr_cube->getComponent<ShaderProgram>().setUniform(str1.c_str(), m_point_lights[i]->getComponent<Transform>().position);	// set shader uniform for lightPosition[i]

		//	std::string str2 = "lightColors[" + std::to_string(i) + "]";
		//	m_pbr_cube->getComponent<ShaderProgram>().setUniform(str2.c_str(), m_point_lights[i]->getComponent<LightObject>().color);		// set shader uniform for lightColors[i]
		//}


	}


	void Shader_PBR_Uniforms::OnImGuiRender() {
		ImGui::Indent();
		if (ImGui::CollapsingHeader("public Uniforms", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
			ImGui::ColorPicker3("Base Color Picker", &_baseColor.r);
		}
		ImGui::Unindent();
	}

	Shader_PBR_Uniforms::Shader_PBR_Uniforms() {
		_pbr_textures = std::make_unique<Texture2D[]>(5);
		_pbr_textures[0].loadTexture("paint/Painted_metal_02_1K_Base_Color.png", true);	//albedo
		_pbr_textures[1].loadTexture("paint/Painted_metal_02_1K_Normal.png", true);		//normal
		_pbr_textures[2].loadTexture("paint/Painted_metal_02_1K_Metallic.png", true);		//metalic
		_pbr_textures[3].loadTexture("paint/Painted_metal_02_1K_Roughness.png", true);	//roughness
		_pbr_textures[4].loadTexture("paint/Painted_metal_02_1K_AO.png", true);			//ambient occlusion 
	}
	Shader_PBR_Uniforms::~Shader_PBR_Uniforms() {}
}

#endif // !SHADER_PBR_UNIFORMS_H
