#ifndef	SHADER_PROGRAM_H
#define	SHADER_PROGRAM_H

#include <string>
#define GLEW_STATIC
#include<GL/glew.h>
#include"glm/glm.hpp"
#include<map>

class ShaderProgram
{

public :
	ShaderProgram();
	~ShaderProgram();

	enum ShaderType {
		VERTEX,
		FRAGMENT,
		PROGRAM
	};

	bool loadShaders(const char* vsFileNamen, const char* fsFileName);
	void use();

	void setUniform(const GLchar* name, const glm::vec2& v);
	void setUniform(const GLchar* name, const glm::vec3& v);
	void setUniform(const GLchar* name, const glm::vec4& v);
	void setUniform(const GLchar* name, const glm::mat4& m);

	GLuint getProgram()const;

private:

	std::string fileToString(const std::string& fileName);
	void checkCompileErrors(GLuint shader, ShaderType type);
	GLint getUniformLocation(const GLchar* name);

	GLuint m_Handle;

	std::map<std::string, GLint> m_UniformLocations;

};

#endif // ! SHADER_PROGRAM_H


