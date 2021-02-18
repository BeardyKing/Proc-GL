#ifndef SHADER_SHADOWDEPTH_H
#define SHADER_SHADOWDEPTH_H
#include "Shader_UniformSetter.h"
#include "../Engine/ECS_List.h"

namespace uniform {
	class Shader_ShadowDepth : public Shader_Uniforms
	{
	public:
		Shader_ShadowDepth();
		~Shader_ShadowDepth();
		void SetUniformMVP(glm::mat4& model, glm::mat4& view, glm::mat4& projection, ShaderProgram& _shader, Camera& _camera);
		void SetUniformCustom(ShaderProgram& _shader)override;
		void OnImGuiRender()override;

	public: //helperFunction
		void SetBaseColor(glm::vec3 _colour) override;
	private:
		glm::vec3 _baseColor = glm::vec3(1.0f,0.38f,0.38f);
	};

	void Shader_ShadowDepth::SetBaseColor(glm::vec3 _color) { _baseColor = _color; }


	void Shader_ShadowDepth::SetUniformMVP(glm::mat4& model, glm::mat4& view, glm::mat4& projection, ShaderProgram& _shader, Camera& _camera) {
		_shader.setUniform("model", model);
		_shader.setUniform("view", view);
		_shader.setUniform("projection", projection);
	}

	void Shader_ShadowDepth::SetUniformCustom(ShaderProgram& _shader){

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, GetShadowMap());

		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 0.01f, far_plane = 15.0f;

		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(GetManager()->FindLights()[0]->getComponent<Transform>().position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		// render scene from light's point of view
		//simpleDepthShader.use();
		_shader.use();
		_shader.setUniform("lightSpaceMatrix", lightSpaceMatrix);

		_shader.setUniformSampler("shadowMap", 1);// 0 = albedo
		_shader.setUniform("near_plane", near_plane);
		_shader.setUniform("far_plane", far_plane);
		//std::cout << "HERE" << std::endl;
		/*uniform sampler2D depthMap;
		uniform float near_plane;
		uniform float far_plane;*/
	}

	void Shader_ShadowDepth::OnImGuiRender() {
		ImGui::Indent();
		if (ImGui::CollapsingHeader("public Uniforms", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
			ImGui::ColorPicker3("Base Color Picker", &_baseColor.r);
		}
		ImGui::Unindent();
	}

	Shader_ShadowDepth::Shader_ShadowDepth(){}
	Shader_ShadowDepth::~Shader_ShadowDepth(){}
}

#endif // !SHADER_SHADOWDEPTH_H
