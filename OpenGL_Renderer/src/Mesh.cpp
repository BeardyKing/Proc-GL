#include "Mesh.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "ECS/EntityManager.h"
#include "Camera.h"
#include "ShaderProgram.h"

int G_GetWindowWidth();
int G_GetWindowHeight();
bool G_GetWindowResizeFlag();
extern EntityManager* GetManager();

std::vector<std::string> split(std::string s, std::string t) {
	std::vector<std::string> res;
	while (1){
		int pos = s.find(t);
		if (pos == -1){
			res.push_back(s);
			break;
		}
		res.push_back(s.substr(0, pos));
		s = s.substr(pos + 1, s.size() - pos - 1);
	}
	return res;
}

Mesh::Mesh() 
	: mLoaded(false)
{
	LoadOBJ("objectDefaults/light.obj");
}

Mesh::Mesh(const char* fileName):
mLoaded(false)
{
	LoadOBJ(fileName);
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
}

void Mesh::OnRender(){
	Entity* cam = nullptr;
	EntityManager* mgr = GetManager();

		std::string name;
		for (auto& e : mgr->entities){
			if (e->hasComponent<FPSCamera>()){
				cam = e->getComponent<ObjectData>().entity;
				break;
			}
		}

		


	FPSCamera& camera = cam->getComponent<FPSCamera>();
	Transform& transform = entity->getComponent<Transform>();
	ShaderProgram& shader = entity->getComponent<ShaderProgram>();

	glm::mat4 model, view, projection;

	view = camera.GetViewMatrix();
	if (camera.usingImGuiWindow == false) { // WINDOW
		projection = glm::perspective(glm::radians(camera.getFOV()), (float)G_GetWindowWidth() / (float)G_GetWindowHeight(), 0.1f, 100.0f);
	}
	else { // EDITOR
		projection = glm::perspective(glm::radians(camera.getFOV()), (float)camera.ImGuiWindowSize.x / (float)camera.ImGuiWindowSize.y, 0.1f, 100.0f);
	}

	////----------------------------------//
	glm::vec3 viewPos = camera.GetPosition();
	////----------------------------------//

	model = glm::mat4(1.0f);
	model =
		glm::translate(model, transform.position) *
		glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1, 0, 0)) *
		glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0, 1, 0)) *
		glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0, 0, 1)) *
		glm::scale(model, transform.scale);

	//////----------------------------------//
	//////				MVP					//
	//////----------------------------------//

	shader.use();
	shader.setUniform("lightCol", entity->getComponent<ShaderProgram>().GetBaseColor());
	shader.setUniform("model", model);
	shader.setUniform("view", view);
	shader.setUniform("projection", projection);

	Draw();
}

void Mesh::OnUpdate(){

}

void Mesh::OnImGuiRender(){
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

	ImGui::Begin("Inspector"); {
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
			ImGui::Indent();
			ImGui::Text("Vertex Size");
			std::string str;
			str = std::to_string(sizeof(Vertex));
			ImGui::Text(&str[0]);
			if (ImGui::CollapsingHeader("Layout", ImGuiTreeNodeFlags_AllowItemOverlap)) {
				ImGui::Text("Position");
				ImGui::Text("0");

				ImGui::Text("Normal");
				std::string editor_normal = std::to_string((3 * sizeof(GLfloat)));
				ImGui::Text(&editor_normal[0]);

				ImGui::Text("Texture coordinates");
				std::string editor_coordinates = std::to_string((6 * sizeof(GLfloat)));
				ImGui::Text(&editor_coordinates[0]);
			}
			ImGui::Unindent();

		}

		ImGui::Separator();
	}
	ImGui::End();
}

void Mesh::OnExit(){

}

bool Mesh::LoadOBJ(const std::string& fileName) {

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempUVs;
	std::vector<glm::vec3> tempNormals;

	if (fileName.find(".obj") != std::string::npos){
		std::ifstream fin(fileName, std::ios::in);
		if (!fin){
			std::cerr << "Can't open : " << fileName << " ..." << std::endl;
			return false;
		}

		std::cout << "Loading OBJ : " << fileName << " ..." << std::endl;

		std::string lineBuffer;
		while (std::getline(fin, lineBuffer)){
			std::stringstream ss(lineBuffer);
			std::string cmd;
			ss >> cmd;

			if (cmd == "v"){
				glm::vec3 vertex;
				int dim = 0;
				while (dim < 3 && ss >> vertex[dim]){
					dim++;
				}
				tempVertices.push_back(vertex);

			}
			else if (cmd == "vt") {
				glm::vec2 uv; 
				int dim = 0;
				while (dim < 2 && ss >> uv[dim]) {
					dim++;
				}
				tempUVs.push_back(uv);

			}
			else if (cmd == "vn") {
				glm::vec3 normal;
				int dim = 0;
				while (dim < 3 && ss >> normal[dim]) {
					dim++;
				}

				normal = glm::normalize(normal);
				tempNormals.push_back(normal);

			}
			else if (cmd == "f") {

				std::string faceData;
				int vertexIndex, uvIndex, normalIndex; //v/vt/vn
				
				while (ss >> faceData){
					std::vector<std::string> data = split(faceData, "/");
					// vertex index
					if (data[0].size() > 0){
						sscanf_s(data[0].c_str(), "%d", &vertexIndex);
						vertexIndices.push_back(vertexIndex);
					}
					//if the face vertex has a texture coord index
					if (data.size() >= 1){
						if (data[1].size() > 0){
							sscanf_s(data[1].c_str(), "%d", &uvIndex);
							uvIndices.push_back(uvIndex);
						}
					}
					// does the face vertex have a normal index
					if (data.size() >= 2) {
						if (data[2].size() > 0) {
							sscanf_s(data[2].c_str(), "%d", &normalIndex);
							normalIndices.push_back(normalIndex);
						}
					}
				}
			}
		}

		//close file
		fin.close();

		std::cout << "finished Reading : " << fileName << std::endl;

		for (unsigned int i = 0; i < vertexIndices.size(); i++) {
			Vertex meshVertex;
			if (tempVertices.size() > 0){
				glm::vec3 vertex = tempVertices[vertexIndices[i] - 1];
				meshVertex.position = vertex;
			}

			if (tempNormals.size() > 0){
				glm::vec3 normal = tempNormals[normalIndices[i] - 1];
				meshVertex.normal = normal;
			}

			if (tempUVs.size() > 0){
				glm::vec2 uv = tempUVs[uvIndices[i] - 1];
				meshVertex.texCoords = uv;
			}

			mVertices.push_back(meshVertex);
		}

		std::cout << "Init Buffers : " << fileName << std::endl;
		InitBuffers();

		return (mLoaded = true);
	}

	return false;
}

void Mesh::Draw() {
	if (!mLoaded) { return; }

	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
	glBindVertexArray(0);
}

void Mesh::InitBuffers() {
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	// position attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL); //3 = vec3
	glEnableVertexAttribArray(0);

	//normals attrib
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));//2 = vec2 // 3 * sizeof(GLfloat) is the offset
	glEnableVertexAttribArray(1);
	// tex Coord 
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));//3 = vec3 //6 * sizeof(GLfloat) is the offset 
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}
