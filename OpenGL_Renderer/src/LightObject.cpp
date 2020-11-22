#include "LightObject.h"

LightObject::LightObject() 
:
	lightType(LightType::Point),
	m_vertexShaderPath("objectDefaults/basic.vert"),
	m_fragmentShaderPath("objectDefaults/basic.frag"),
	m_meshPath("objectDefaults/light.obj"),
	color(glm::vec3(300.0f, 150.0f, 150.0f)),
	direction(glm::vec3(0.0f, -0.9f, -0.17f))
{
	m_Shader = std::make_unique<ShaderProgram>();
	m_Shader->loadShaders(m_vertexShaderPath, m_fragmentShaderPath);

	m_Mesh = std::make_unique<Mesh>();
	m_Mesh->LoadOBJ(m_meshPath);
}

LightObject::~LightObject() {

}

void LightObject::LoadMesh(const char* meshPath) {

}

void LightObject::LoadShader(const char* vertPath, const char* fragPath) {

}

void LightObject::OnRender()
{

}

void LightObject::OnUpdate(double deltaTime)
{

}

void LightObject::OnImGuiRender(){
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
	ImGui::Begin("Inspector");
	{
		ImGui::Separator();
		ImGui::Text("Light Object");
		ImGui::ColorPicker3("Light Color", &color.r);
		//ImGui::CollapsingHeader("HELLO FROM COL");

		ImGui::Separator();
	}
	ImGui::End();
}

