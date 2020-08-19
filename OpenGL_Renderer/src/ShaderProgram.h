#ifndef	SHADER_PROGRAM_H
#define	SHADER_PROGRAM_H

#include <string>
#define GLEW_STATIC
#include<GL/glew.h>

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

private:

	std::string fileToString(const std::string& fileName);
	void checkCompileErrors(GLuint shader, ShaderType type);

	GLuint m_Handle;

};

#endif // ! SHADER_PROGRAM_H


