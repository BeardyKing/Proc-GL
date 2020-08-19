#include "ShaderProgram.h"
#include <fstream>
#include <iostream>
#include <sstream>

ShaderProgram::ShaderProgram() 
	: m_Handle(0)
{

}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(m_Handle);
}

bool ShaderProgram::loadShaders (const char* vsFileName, const char* fsFileName) {


	std::string vsString = fileToString(vsFileName);
	std::string fsString = fileToString(fsFileName);
	const GLchar* vsSourcePtr = vsString.c_str();
	const GLchar* fsSourcePtr = fsString.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	
	glShaderSource(vs, 1, &vsSourcePtr, NULL);
	glShaderSource(fs, 1, &fsSourcePtr, NULL);

	glCompileShader(vs);
	checkCompileErrors(vs, VERTEX);
	glCompileShader(fs);
	checkCompileErrors(fs, FRAGMENT);

	m_Handle = glCreateProgram();
	glAttachShader(m_Handle, vs);
	glAttachShader(m_Handle, fs);
	glLinkProgram(m_Handle);

	checkCompileErrors(m_Handle, PROGRAM);

	glDeleteShader(vs);
	glDeleteShader(fs);

	//uniform location clear

	return true;
}

void ShaderProgram::use() {
	if (m_Handle > 0){
		glUseProgram(m_Handle);
	}
}

std::string ShaderProgram::fileToString(const std::string& fileName) {
	std::stringstream ss;
	std::ifstream file;

	try{
		file.open(fileName, std::ios::in);

		if (!file.fail()){
			ss << file.rdbuf();
		}
		
		file.close();
	}
	catch (std::exception ex){
		std::cerr << "Error reading shader filename" << std::endl;
	}

	//std::cout << ss.str() << std::endl;

	return ss.str();
}

void ShaderProgram::checkCompileErrors(GLuint shader, ShaderType type) {
	int status = 0;

	if (type == PROGRAM){
		glGetProgramiv(m_Handle, GL_LINK_STATUS, &status);
		if (status == GL_FALSE){
			GLint length = 0;
			glGetProgramiv(m_Handle, GL_INFO_LOG_LENGTH, &length);

			std::string errorLog(length, ' ');
			glGetProgramInfoLog(m_Handle, length, &length, &errorLog[0]);
			std::cerr << "ERROR! Program failed to link : " << errorLog << std::endl;
		}
	}
	else{ // VERTEX OR FRAGMENT COMPILING
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE){
			GLint length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

			std::string errorLog(length, ' ');
			glGetShaderInfoLog(shader, length, &length, &errorLog[0]);
			std::cerr << "ERROR! Shdaer failed to compile : " << errorLog << std::endl;
		}
	}
}