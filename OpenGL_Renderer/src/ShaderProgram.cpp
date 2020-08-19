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

	m_UniformLocations.clear();

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

GLint ShaderProgram::getUniformLocation(const GLchar* name) { // check if uniform has already been found
	std::map<std::string, GLint>::iterator it = m_UniformLocations.find(name);

	if (it == m_UniformLocations.end()){
		m_UniformLocations[name] = glGetUniformLocation(m_Handle, name);
	}

	return m_UniformLocations[name];
}

void ShaderProgram::setUniform(const GLchar* name, const glm::vec2& v) {
	GLint loc = getUniformLocation(name);
	glUniform2f(loc, v.x, v.y);
}

void ShaderProgram::setUniform(const GLchar* name, const glm::vec3& v) {
	GLint loc = getUniformLocation(name);
	glUniform3f(loc, v.x, v.y, v.z);
}

void ShaderProgram::setUniform(const GLchar* name, const glm::vec4& v) {
	GLint loc = getUniformLocation(name);
	glUniform4f(loc, v.x, v.y, v.z, v.w);
}
