#ifndef	SHADER_PROGRAM_H
#define	SHADER_PROGRAM_H

#include <string>
#define GLEW_STATIC
#include<GL/glew.h>
#include"glm/glm.hpp"
#include<map>
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "Engine/Shader_UniformSetter.h"

class ShaderProgram : public Component
{
public :
	ShaderProgram();
	ShaderProgram(const char* _vsFileName, const char* _fsFileName);
	ShaderProgram(const char* _vsFileName, const char* _fsFileName, const char* shader_uniform_name);
	
	~ShaderProgram();

	void LoadShaderMenu();

	void OnImGuiRender();
	glm::vec3 GetBaseColor();
	void SetBaseColor(glm::vec3 color);

	enum ShaderType {
		VERTEX,
		FRAGMENT,
		PROGRAM
	};

	uniform::Shader_Uniforms* currentShader_uniform;
	uniform::ShaderMenu* shaderMenu;

	bool loadShaders(const char* vsFileName, const char* fsFileName);
	void use();

	void setUniform(const GLchar* name, const glm::vec2& v);
	void setUniform(const GLchar* name, const glm::vec3& v);
	void setUniform(const GLchar* name, const glm::vec4& v);
	void setUniform(const GLchar* name, const glm::mat4& m);
	void setUniform(const GLchar* name, const GLfloat& f);
	void setUniform(const GLchar* name, const GLint& i);

	void setUniformSampler(const GLchar* name, const GLint& slot);

	GLuint getProgram()const;

private:

	std::string editor_vertexShader;
	std::string editor_fragmentShader;

	glm::vec3 m_BaseColor;

	std::string fileToString(const std::string& fileName);
	void checkCompileErrors(GLuint shader, ShaderType type);
	GLint getUniformLocation(const GLchar* name);

	GLuint m_Handle;

	std::map<std::string, GLint> m_UniformLocations;
};

#endif // ! SHADER_PROGRAM_H


