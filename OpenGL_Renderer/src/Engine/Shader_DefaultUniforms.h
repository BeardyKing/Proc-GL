#ifndef SHADER_DEFAULTUNIFORMS_H
#define SHADER_DEFAULTUNIFORMS_H
#include "Shader_UniformSetter.h"
#include "../Engine/ECS_List.h"

namespace uniform {
	class Shader_DefaultUniforms : public Shader_Uniforms
	{
	public:
		Shader_DefaultUniforms();
		~Shader_DefaultUniforms();
		void SetUniformMVP(glm::mat4& model, glm::mat4& view, glm::mat4& projection, ShaderProgram& _shader, Camera& _camera);
		void SetUniformCustom(ShaderProgram& _shader)override;
		void OnImGuiRender()override;

	public: //helperFunction
		void SetBaseColor(glm::vec3 _colour) override;
	private:
		glm::vec3 _baseColor = glm::vec3(1.0f,0.38f,0.38f);
	};

	void Shader_DefaultUniforms::SetBaseColor(glm::vec3 _color) { _baseColor = _color; }


	void Shader_DefaultUniforms::SetUniformMVP(glm::mat4& model, glm::mat4& view, glm::mat4& projection, ShaderProgram& _shader, Camera& _camera) {
		_shader.setUniform("model", model);
		_shader.setUniform("view", view);
		_shader.setUniform("projection", projection);
	}

	void Shader_DefaultUniforms::SetUniformCustom(ShaderProgram& _shader){
		_shader.setUniform("lightCol", _baseColor);
	}

	void Shader_DefaultUniforms::OnImGuiRender() {
		ImGui::Indent();
		if (ImGui::CollapsingHeader("public Uniforms", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
			ImGui::ColorPicker3("Base Color Picker", &_baseColor.r);
		}
		ImGui::Unindent();
	}

	Shader_DefaultUniforms::Shader_DefaultUniforms(){}
	Shader_DefaultUniforms::~Shader_DefaultUniforms(){}
}

#endif // !SHADER_DEFAULTUNIFORMS_H
