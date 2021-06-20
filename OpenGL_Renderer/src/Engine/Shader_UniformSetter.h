#ifndef SHADER_UNIFORMSETTER_H
#define SHADER_UNIFORMSETTER_H

#include "../ECS/Component.h"
#include "../ECS/Entity.h"
//#include "ECS_List.h"


#include <iostream>
#include <vector>
#include <string>
#include <functional>

class ShaderProgram;
class Camera;

namespace uniform {


	class Shader_Uniforms : public Component {
	public:
		Shader_Uniforms() {}
		virtual ~Shader_Uniforms() {}
		virtual void LoadTextures(Entity& _shader) {}

		virtual void SetUniformMVP(glm::mat4& model, glm::mat4& view, glm::mat4& projection, ShaderProgram& _shader, Camera& _camera) {
			std::cout << "base MVP" << std::endl;
		}
		virtual void SetUniformCustom(ShaderProgram& _shader) {}

	public://helper functions
		virtual void SetBaseColor(glm::vec3 _color) {}
		virtual void SetTextureScale(glm::vec2 _scale) {}

		virtual void SetColour(const glm::vec4& value, const std::string& name) {}
		virtual void SetFloat(const float& value, const std::string& name) {}
		virtual void SetInt(const int& value, const std::string& name) {}
	};

	class ShaderMenu : public Shader_Uniforms {
	public:
		ShaderMenu(Shader_Uniforms*& currentShaderPointer);


		template<typename T>
		void RegisterShader(const std::string& name)
		{
			std::cout << "register shader : " << name << std::endl;
			_shader_uniform.push_back(std::make_pair(name, []() {return new T(); }));
		}

		Shader_Uniforms* SelectShader(int shaderToSelect) {
			return _shader_uniform[shaderToSelect].second();
		}

		Shader_Uniforms* SelectShader(std::string shaderToSelect) {
			for (auto& _shader : _shader_uniform) {
				if (_shader.first == shaderToSelect){
					return _shader.second();
				}
			}
			std::cout << "--- ERROR : Shader_UniformSetter.cpp - Did not find shader_uniform with name :" << shaderToSelect << std::endl;
			std::cout << "--- ERROR : Shader_UniformSetter.cpp - Setting Shader_Uniform to basic fallback" << std::endl;
			return _shader_uniform[0].second();
		}



	private:
		Shader_Uniforms*& m_CurrentShader;
		std::vector<std::pair<std::string, std::function<Shader_Uniforms* ()>>> _shader_uniform;
	};

}

#endif // !SHADER_UNIFORMSETTER_H


