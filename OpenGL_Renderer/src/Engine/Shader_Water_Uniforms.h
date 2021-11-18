#ifndef SHADER_WATER_UNIFORMS_H
#define SHADER_WATER_UNIFORMS_H

#include "Engine_Core.h"
#include "Engine_UtilityFunctions.h"

#include <vector>

namespace uniform {
	class Shader_Water_Uniforms : public Shader_Uniforms
	{
	public:
		Shader_Water_Uniforms();
		~Shader_Water_Uniforms();

		void LoadTextures(Entity& _shader);

		void SetUniformMVP(glm::mat4& model, glm::mat4& view, glm::mat4& projection, ShaderProgram& _shader, Camera& _camera);
		void SetUniformCustom(ShaderProgram& _shader)override;
		void OnImGuiRender()override;

	public: //helperFunction
		void SetBaseColor(glm::vec3 _colour) override;
		void SetTextureScale(glm::vec2 _scale) override;
		void SetColour(const glm::vec4& value, const std::string& name) override;
		void SetFloat(const float& value, const std::string& name) override;
		void SetInt(const int& value, const std::string& name) override;

	private:
		std::unique_ptr <Texture2D[]> m_pbr_textures;
		int numberOfTextures = 0;
	public:
		glm::vec3 m_baseColor = glm::vec3(1.0f, 0.38f, 0.38f);
		glm::vec2 m_TextureTiling = glm::vec2(1);
		bool castShadows = true;
		bool recieveShadows = true;
		float specular = 0;

		glm::vec4 albedo_color	= glm::vec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
		float normal_scalar		= 1.0f;
		float metallic_scalar	= 1.0f;
		float roughness_scalar	= 1.0f;
		float occlusion_scalar	= 1.0f;

		float waterDepthBlend = 5.0f;
	};

	void Shader_Water_Uniforms::SetBaseColor(glm::vec3 _color) { m_baseColor = _color; }
	void Shader_Water_Uniforms::SetTextureScale(glm::vec2 _scale) { m_TextureTiling = _scale; }

	 void Shader_Water_Uniforms::SetColour(const glm::vec4& value, const std::string& name) {
		 if (name == "albedo_color"){
			 albedo_color = value;
		 }
		 else{
			 std::cout << " could not find uniform with name " << name << " of type glm::vec4 "<< std::endl;
		 }
	 }
	 
	 void Shader_Water_Uniforms::SetFloat(const float& value, const std::string& name) {
		if (name == "normal_scalar") {	 normal_scalar = value;
		}
		else if (name == "metallic_scalar"){	 
			metallic_scalar = value;
		}
		else if (name == "roughness_scalar"){	 
			roughness_scalar = value;
		}
		else if (name == "occlusion_scalar"){	 
			occlusion_scalar = value;
		}
		else if (name == "waterDepthBlend") {
			waterDepthBlend = value;
		}
		else {	 
			std::cout << " could not find uniform with name " << name << " of type float " << std::endl;
		}
	 }

	 void Shader_Water_Uniforms::SetInt(const int& value, const std::string& name) {
		std::cout << " could not find uniform with name " << name << " of type int " << std::endl;
	 }


	void Shader_Water_Uniforms::SetUniformMVP(glm::mat4& model, glm::mat4& view, glm::mat4& projection, ShaderProgram& _shader, Camera& _camera) {
		//glm::vec3 cameraPosition = G_GetManager()->FindActiveCamera()->getComponent<Transform>().position; // Also viable
		glm::mat4 _newModel = glm::mat4(1.0f);
		_shader.setUniform("model", model);
		_shader.setUniform("view", view);
		_shader.setUniform("projection", projection);
		_shader.setUniform("camPos", _camera.GetPosition());
	}

	void Shader_Water_Uniforms::SetUniformCustom(ShaderProgram& shader) {


		auto camPos = G_GetManager()->FindActiveCamera()->getComponent<Transform>().position;

		shader.use();

		m_pbr_textures[0].Bind(0);
		m_pbr_textures[1].Bind(1);
		m_pbr_textures[2].Bind(2);
		m_pbr_textures[3].Bind(3);
		m_pbr_textures[4].Bind(4);

		glActiveTexture(GL_TEXTURE0 + 5);
		glBindTexture(GL_TEXTURE_2D, G_GetShadowMap()); // bind shadowmap texture(s)

		auto m_lights = G_GetManager()->FindLights();
		auto sb = G_GetManager()->FindEntityWithType<SkyBox>();

		glBindTexture(GL_TEXTURE_CUBE_MAP, sb->getComponent<SkyBox>().GetSkyboxTexture()); // instead of _shader.bind
		
		// renderpass Depth
		glActiveTexture(GL_TEXTURE0 + 6);
		glBindTexture(GL_TEXTURE_2D, G_GetRenderPass().GetDepthBuffer()); // bind depth pass texture(s)

		// renderpass colour
		glActiveTexture(GL_TEXTURE0 + 7);
		glBindTexture(GL_TEXTURE_2D, G_GetRenderPass().GetRenderBuffer()); // bind depth pass texture(s)


		shader.setUniform("waterDepthBlend",waterDepthBlend);

		shader.setUniform("amountOfLights",		(GLint)m_lights.size());
		shader.setUniform("textureScale",			m_TextureTiling);
		shader.setUniform("_shadowIntensity",		m_lights[0]->getComponent<LightObject>().shadowIntensity); // todo write fallback for if no lights
		shader.setUniform("_lightIntensity",		m_lights[0]->getComponent<LightObject>().lightIntensity);
		shader.setUniform("_lightColor",			m_lights[0]->getComponent<LightObject>().color);
		shader.setUniform("viewPos",				camPos);

		shader.setUniform("albedo_color",			albedo_color);
		shader.setUniform("normal_scalar",			normal_scalar);
		shader.setUniform("metallic_scalar",		metallic_scalar);
		shader.setUniform("roughness_scalar",		roughness_scalar);
		shader.setUniform("occlusion_scalar",		occlusion_scalar);

		shader.setUniformSampler("albedoMap",				0);		// 0 = albedo
		shader.setUniformSampler("normalMap",				1);		// 1 = normal
		shader.setUniformSampler("metallicMap",				2);		// 2 = metalic
		shader.setUniformSampler("roughnessMap",			3);		// 3 = roughness
		shader.setUniformSampler("aoMap",					4);		// 4 = ambient 
		shader.setUniformSampler("shadowMap",				5);		// 5 = shadow
		shader.setUniformSampler("cameraDepthRenderPass",	6);
		shader.setUniformSampler("cameraColorRenderPass",	7);

		
		//std::cout << m_lights[0]->getComponent<LightObject>().color.x << "," 
		//<< m_lights[0]->getComponent<LightObject>().color.y << " , " 
		//<< m_lights[0]->getComponent<LightObject>().color.z << std::endl;
		
		for (unsigned int i = 0; i < m_lights.size(); ++i) {
			std::string str1 = "lightPositions[" + std::to_string(i) + "]";
			shader.setUniform(str1.c_str(), m_lights[i]->getComponent<Transform>().position);	// set shader uniform for lightPosition[i]

			std::string str2 = "lightColors[" + std::to_string(i) + "]";
			shader.setUniform(str2.c_str(), m_lights[i]->getComponent<LightObject>().color);	// set shader uniform for lightColors[i]
		}

		if (G_RenderShadowMap() && castShadows) {
			shader.setUniform("view", glm::mat4(1));
			shader.setUniform("projection", m_lights[0]->getComponent<LightObject>().LightSpaceMatrix());
		}
		if (recieveShadows) {
			shader.setUniform("lightSpaceMatrix", m_lights[0]->getComponent<LightObject>().LightSpaceMatrix());
		}

	}

	void Shader_Water_Uniforms::OnImGuiRender() {
		
	#pragma region EDITOR_SURFACE_INPUTS

		ImGui::SetNextTreeNodeOpen(true, ImGuiTreeNodeFlags_DefaultOpen);
		if (ImGui::TreeNode("Surface Options")){
			ImGui::Columns(2, "Surface_Options_Columns", false);
			
			ImGui::Selectable("Cast Shadows");
			ImGui::NextColumn();
			ImGui::Checkbox("##Cast Shadows", &castShadows);
			
			ImGui::NextColumn();
			
			ImGui::Selectable("Recieve Shadows");
			ImGui::NextColumn();
			ImGui::Checkbox("##Recieve Shadows", &recieveShadows);
			ImGui::NextColumn();
			
			ImGui::Selectable("Edge Blend Distance");
			ImGui::NextColumn();
			ImGui::SliderFloat("##Edge Blend Distance", &waterDepthBlend, 0.0f, 30.0f);

			
			ImGui::Columns(1);
			ImGui::TreePop();

		}
		ImGui::Separator();

	#pragma endregion

	#pragma region EDITOR_SURFACE_INPUTS

		ImGui::SetNextTreeNodeOpen(true, ImGuiTreeNodeFlags_DefaultOpen);
		if (ImGui::TreeNode("Surface Inputs")) {

			ImGui::Columns(2, "Surface_Inputs_Columns", false);
			//--------------Albedo Map--------------//
			static uint16_t albedoMap_Image_Scalar = 1;
			if(ImGui::ImageButton((void*)m_pbr_textures[0].GetTexture(), ImVec2(12.0f * albedoMap_Image_Scalar, 12.0f * albedoMap_Image_Scalar))) {
				albedoMap_Image_Scalar = (albedoMap_Image_Scalar == 1) ? 10 : 1;
			}
			ImGui::SameLine();
			ImGui::Selectable("Albedo Map");
			ImGui::NextColumn();
			if (ImGui::ColorButton("albedo_color", ImVec4(albedo_color.x, albedo_color.y, albedo_color.z, albedo_color.w))) {
				ImGui::SetNextWindowPos(ImVec2(ImGui::GetMousePos().x - 320, ImGui::GetMousePos().y - 120));
				ImGui::OpenPopup("albedo_color_picker");
			}
			if (ImGui::BeginPopup("albedo_color_picker", ImGuiSelectableFlags_DontClosePopups)){
				ImGui::ColorPicker4("##picker", &albedo_color.x, ImGuiColorEditFlags_None , NULL);
				ImGui::EndPopup();
			}
			ImGui::NextColumn();

			//--------------Normal Map--------------//
			 static uint16_t normalMap_Image_Scalar = 1;
			if (ImGui::ImageButton((void*)m_pbr_textures[1].GetTexture(), ImVec2(12.0f * normalMap_Image_Scalar, 12.0f * normalMap_Image_Scalar))) {
				normalMap_Image_Scalar = (normalMap_Image_Scalar == 1) ? 10 : 1;
			}

			ImGui::SameLine();
			ImGui::Selectable("Normal Map");

			ImGui::NextColumn();
			ImGui::SliderFloat("##normal_scalar", &normal_scalar, 0.3f, 90.0f);
			ImGui::NextColumn();

			//--------------Metallic Map--------------//
			static uint16_t metallicMap_Image_Scalar = 1;
			if (ImGui::ImageButton((void*)m_pbr_textures[2].GetTexture(), ImVec2(12.0f * metallicMap_Image_Scalar, 12.0f * metallicMap_Image_Scalar))) {
				metallicMap_Image_Scalar = (metallicMap_Image_Scalar == 1) ? 10 : 1;
			}

			ImGui::SameLine();
			ImGui::Selectable("Metallic Map");

			ImGui::NextColumn();
			ImGui::SliderFloat("##Metallic_scalar", &metallic_scalar, 0.0f, 1.0f);
			ImGui::NextColumn();

			//--------------Roughness Map--------------//
			static uint16_t RoughnessMap_Image_Scalar = 1;
			if (ImGui::ImageButton((void*)m_pbr_textures[3].GetTexture(), ImVec2(12.0f * RoughnessMap_Image_Scalar, 12.0f * RoughnessMap_Image_Scalar))) {
				RoughnessMap_Image_Scalar = (RoughnessMap_Image_Scalar == 1) ? 10 : 1;
			}

			ImGui::SameLine();
			ImGui::Selectable("Roughness Map");

			ImGui::NextColumn();
			ImGui::SliderFloat("##Roughness_scalar", &roughness_scalar, 0.0f, 4.0f);
			ImGui::NextColumn();

			//--------------Occlusion Map--------------//
			static uint16_t OcclusionMap_Image_Scalar = 1;
			if (ImGui::ImageButton((void*)m_pbr_textures[4].GetTexture(), ImVec2(12.0f * OcclusionMap_Image_Scalar, 12.0f * OcclusionMap_Image_Scalar))) {
				OcclusionMap_Image_Scalar = (OcclusionMap_Image_Scalar == 1) ? 10 : 1;
			}

			ImGui::SameLine();
			ImGui::Selectable("Occlusion Map");

			ImGui::NextColumn();
			ImGui::SliderFloat("##Occlusion_scalar", &occlusion_scalar, 0.0f, 10.0f);
			ImGui::NextColumn();

			//--------------Shadow Map--------------// // TODO Update to support multiple shadowmaps
			static uint16_t ShadowMap_Image_Scalar = 1;
			if (ImGui::ImageButton((void*)G_GetShadowMap(), ImVec2(12.0f * ShadowMap_Image_Scalar, 12.0f * ShadowMap_Image_Scalar))) {
				ShadowMap_Image_Scalar = (ShadowMap_Image_Scalar == 1) ? 10 : 1;
			}
			ImGui::SameLine();
			ImGui::Selectable("Shadow Map");

			ImGui::Columns(1);
			ImGui::TreePop();
		}
		ImGui::Separator();

	#pragma endregion

	#pragma region EDITOR_ADVANCED_OPTIONS

		ImGui::SetNextTreeNodeOpen(true, ImGuiTreeNodeFlags_DefaultOpen);
		if (ImGui::TreeNode("Advanced_Options")) {
			ImGui::Columns(2, "Advanced_Options_Columns", false);
			
			ImGui::Selectable("Texture Tiling");
			ImGui::NextColumn();
			ImGui::DragFloat2("Texture Tiling", &m_TextureTiling.x, -0.0125f, 0.0125f);
			
			ImGui::Columns(1);
			ImGui::TreePop();

		}
		ImGui::Separator();

	#pragma endregion

	}

	void Shader_Water_Uniforms::LoadTextures(Entity& _shader) {
		auto tex = _shader.getComponent<ShaderProgram>().GetTextures();
		numberOfTextures = tex.size();
		m_pbr_textures = std::make_unique<Texture2D[]>(numberOfTextures);

		for (int i = 0; i < numberOfTextures; i++) {
			m_pbr_textures[i].LoadTexture(tex[i], true);
		}
	}

	Shader_Water_Uniforms::Shader_Water_Uniforms() {}
	Shader_Water_Uniforms::~Shader_Water_Uniforms() {}
}

#endif // !SHADER_STANDARD_LIT_UNIFORMS_H
