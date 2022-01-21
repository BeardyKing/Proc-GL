#include "ShaderProgram.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "glm/gtc/type_ptr.hpp"

#include "Shader_Standard_Lit_Uniforms.h"
#include "Shader_DefaultUniforms.h"
#include "Shader_PBR_Uniforms.h"
#include "Shader_Blinn_Phong_Uniforms.h"
#include "Shader_ShadowDepth.h"
#include "Shader_Standard_Skybox_Uniforms.h"
#include "Shader_Terrain_Uniforms.h"
#include "Shader_Water_Uniforms.h"
#include "Shader_Post_Processing_Uniforms.h"

ShaderProgram::ShaderProgram()
	: m_Handle(0)
	//m_BaseColor(glm::vec3(300.0f, 150.0f, 150.0f))
{
	loadShaders("objectDefaults/basic.vert", "objectDefaults/basic.frag");
	LoadShaderMenu();

	currentShader_uniform = shaderMenu->SelectShader(0);

}


ShaderProgram::ShaderProgram(const char* _vsFileName, const char* _fsFileName)
	: m_Handle(0)
{
	loadShaders(_vsFileName, _fsFileName);
	LoadShaderMenu();

	currentShader_uniform = shaderMenu->SelectShader(0);
}

ShaderProgram::ShaderProgram(const char* _vsFileName, const char* _fsFileName, std::string shader_uniform_name)
	: m_Handle(0)
{
	loadShaders(_vsFileName, _fsFileName);
	LoadShaderMenu();

	currentShader_uniform = shaderMenu->SelectShader(shader_uniform_name);
	
	
	
	//currentShader_uniform->Uniform_Init();
	//ShaderProgram& shader = entity->getComponent<ShaderProgram>();

}

void ShaderProgram::LoadShaderMenu() {
	currentShader_uniform = nullptr;
	shaderMenu = new uniform::ShaderMenu(currentShader_uniform);
	currentShader_uniform = shaderMenu;	// TODO ADD SHADER LIST LOADER
	shaderMenu->RegisterShader<uniform::Shader_DefaultUniforms>			("Uniform_basic"); 
	shaderMenu->RegisterShader<uniform::Shader_ShadowDepth>				("Uniform_ShadowDepth"); 
	shaderMenu->RegisterShader<uniform::Shader_Standard_Lit_Uniform>	("Uniform_Standard_Lit");
	shaderMenu->RegisterShader<uniform::Shader_Standard_Skybox_Uniforms>("Uniform_Skybox");
	shaderMenu->RegisterShader<uniform::Shader_Terrain_Uniforms>		("Uniform_Standard_Terrain");
	shaderMenu->RegisterShader<uniform::Shader_Water_Uniforms>			("Uniform_Standard_Water");
	shaderMenu->RegisterShader<uniform::Shader_Post_Processing_Uniforms>("Uniform_Post_Processing");

}

void ShaderProgram::AddTexturePath(std::string _textureFileName) {
	texturePaths.emplace_back(_textureFileName);
}

void ShaderProgram::LoadTextures() {
	currentShader_uniform->LoadTextures(*entity);
}

void ShaderProgram::AddTexturePath(std::vector<std::string> _textureFileNames) {
	texturePaths = _textureFileNames;
}

std::vector<std::string> ShaderProgram::GetTextures() {
	return texturePaths;
}

ShaderProgram::~ShaderProgram() {
	if (currentShader_uniform != nullptr){
		delete currentShader_uniform;
		currentShader_uniform = nullptr;
	}
	//glDeleteProgram(m_Handle);
}

void ShaderProgram::OnImGuiRender(){
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

	ImGui::Begin("Inspector"); {
		ImGui::Separator();
		if (ImGui::CollapsingHeader("Shader Program", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
			
			if (currentShader_uniform) {
				currentShader_uniform->OnImGuiRender();
			}

			if (ImGui::CollapsingHeader("Vertex Shader", ImGuiTreeNodeFlags_AllowItemOverlap)) {
			ImGuiInputTextFlags flags = ImGuiInputTextFlags_ReadOnly;
			ImGui::InputTextMultiline("EDITOR_VS", &editor_vertexShader[0], editor_vertexShader.size(), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);
			}

			if (ImGui::CollapsingHeader("Fragment Shader",ImGuiTreeNodeFlags_AllowItemOverlap)) {
				ImGuiInputTextFlags flags = ImGuiInputTextFlags_ReadOnly;
				ImGui::InputTextMultiline("EDITOR_FS", &editor_fragmentShader[0], editor_fragmentShader.size(), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);
			}
			ImGui::Unindent();
		}
		ImGui::Separator();
	}
	ImGui::End();
}

void ShaderProgram::OnUpdate(double deltaTime){
	currentShader_uniform->OnUpdate(deltaTime);
}


void ShaderProgram::SetColour(const glm::vec4& value, const std::string& name) {
	currentShader_uniform->SetColour(value, name);
}

void ShaderProgram::SetFloat(const float& value, const std::string& name) {
	currentShader_uniform->SetFloat(value, name);
}

void ShaderProgram::SetInt(const int& value, const std::string& name) {
	currentShader_uniform->SetInt(value, name);
}

void ShaderProgram::SetVec2(const glm::vec2& value, const std::string& name) {
	currentShader_uniform->SetVec2(value, name);
}

void ShaderProgram::SetVec3(const glm::vec3& value, const std::string& name) {
	currentShader_uniform->SetVec3(value, name);
}

void ShaderProgram::SetVec4(const glm::vec4& value, const std::string& name) {
	currentShader_uniform->SetVec4(value, name);
}

void ShaderProgram::SetMat2(const glm::mat2& value, const std::string& name) {
	currentShader_uniform->SetMat2(value, name);
}

void ShaderProgram::SetMat3(const glm::mat3& value, const std::string& name) {
	currentShader_uniform->SetMat3(value, name);
}

void ShaderProgram::SetMat4(const glm::mat4& value, const std::string& name) {
	currentShader_uniform->SetMat4(value, name);
}

void ShaderProgram::SetRenderTexture(GLuint fbo_render_texture){
	currentShader_uniform->SetRenderTexture(fbo_render_texture);
}

void ShaderProgram::SetBaseColor(glm::vec3 color){
	if (currentShader_uniform){
		currentShader_uniform->SetBaseColor(color);
	}
}

void ShaderProgram::SetTextureScale(glm::vec2 scale) {
	if (currentShader_uniform){
		currentShader_uniform->SetTextureScale(scale);
	}
}

bool ShaderProgram::loadShaders (const char* vsFileName, const char* fsFileName) {
	std::string vsString = fileToString(vsFileName);
	std::string fsString = fileToString(fsFileName);
	const GLchar* vsSourcePtr = vsString.c_str();
	const GLchar* fsSourcePtr = fsString.c_str();

	//_EDITOR_

	editor_vertexShader = vsString;
	editor_fragmentShader = fsString;

	//_EDITOR_

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

GLuint ShaderProgram::getProgram()const {
	return m_Handle;
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

void ShaderProgram::setUniform(const GLchar* name, const glm::mat4& m) {
	GLint loc = getUniformLocation(name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}

void ShaderProgram::setUniform(const GLchar* name, const GLfloat& f) {
	GLfloat loc = getUniformLocation(name);
	glUniform1f(loc, f);
}

void ShaderProgram::setUniformSampler(const GLchar* name, const GLint& slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	GLfloat loc = getUniformLocation(name);
	glUniform1i(loc, slot);
}

void ShaderProgram::setUniformSamplerCube(const GLchar* name, const GLint& i) {
	glBindTexture(GL_TEXTURE_CUBE_MAP, i);
}

void ShaderProgram::setUniform(const GLchar* name, const GLint& i) {
	GLint loc = getUniformLocation(name);
	glUniform1i(loc, i);
}


