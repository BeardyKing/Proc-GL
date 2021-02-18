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
		bool blinn = true;
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

		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 0.01f, far_plane = 7.5f * 2;
		//lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
		lightProjection = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, near_plane, far_plane);
		//lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightView = glm::lookAt(m_lights[0]->getComponent<Transform>().position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		// render scene from light's point of view
		// render scene from light's point of view
		//simpleDepthShader.use();
		_shader.use();
		if (RenderShadowMap()){
			_shader.setUniform("view", glm::mat4(1));
			_shader.setUniform("projection", lightSpaceMatrix);
		}
		_shader.setUniform("lightSpaceMatrix", lightSpaceMatrix);
		_shader.setUniformSampler("diffuseTexture",0);// 0 = albedo
		_shader.setUniformSampler("shadowMap",1);// 0 = albedo



		_shader.setUniform("viewPos", camPos);
		_shader.setUniform("lightPos", m_lights[0]->getComponent<Transform>().position);

		



		
	}

	void Shader_Blinn_Phong_Uniforms::OnImGuiRender() {
		ImGui::Indent();
		ImGui::Separator();
		if (ImGui::CollapsingHeader("Public Uniforms",ImGuiTreeNodeFlags_AllowItemOverlap)) {
			ImGui::Indent();

			ImGui::Checkbox("blinn", &blinn);

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
