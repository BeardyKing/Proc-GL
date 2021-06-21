#ifndef	SHADER_PROGRAM_H
#define	SHADER_PROGRAM_H

#include<GL/glew.h>

#include"glm/glm.hpp"

#include <vector>
#include <string>
#include <map>

#include "Shader_UniformSetter.h"
#include "Buffers/DepthBuffer.h"
#include "Buffers/FrameBuffer.h"

class ShaderProgram : public Component
{
public :
	ShaderProgram();
	ShaderProgram(const char* _vsFileName, const char* _fsFileName);
	ShaderProgram(const char* _vsFileName, const char* _fsFileName, std::string shader_uniform_name);
	
	~ShaderProgram();

	void LoadShaderMenu();

	void AddTexturePath(std::string _textureFileName);
	void AddTexturePath(std::vector<std::string> _textureFileNames);
	void LoadTextures();
	std::vector<std::string> GetTextures();
	FrameBuffer shadowMap;
	bool renderShadowMap = false;


	void OnImGuiRender();

	void SetBaseColor(glm::vec3 color);
	void SetTextureScale(glm::vec2 scale);

	void SetColour(const glm::vec4& value, const std::string& name);
	void SetFloat(const float& value, const std::string& name);
	void SetInt(const int& value, const std::string& name);

	glm::vec2 GetTextureScale();
	glm::vec3 GetBaseColor();

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

	std::vector<std::string> texturePaths;


	std::string editor_vertexShader;
	std::string editor_fragmentShader;

	std::string fileToString(const std::string& fileName);
	void checkCompileErrors(GLuint shader, ShaderType type);
	GLint getUniformLocation(const GLchar* name);

	GLuint m_Handle;

	std::map<std::string, GLint> m_UniformLocations;
};

#endif // ! SHADER_PROGRAM_H


