#include "Mesh.h"
#include <iostream>
#include <sstream>
#include <fstream>

Mesh::Mesh() 
	: mLoaded(false)
{

}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
}

bool Mesh::LoadOBJ(const std::string& fileName) {

	std::vector<unsigned int> vertexIndicies, uvIndicies;
	std::vector<glm::vec3> tempVerticies;
	std::vector<glm::vec2> tempUVs;

	if (fileName.find(".obj") != std::string::npos){
		std::ifstream fin(fileName, std::ios::in);
		if (!fin){
			std::cerr << "Can't open " << fileName << " ..." << std::endl;
			return false;
		}

		std::cout << "Loading OBJ file " << fileName << " ..." << std::endl;

		std::string lineBuffer;
		while (std::getline(fin, lineBuffer)){
			if (lineBuffer.substr(0,2) == "v "){
				std::istringstream v(lineBuffer.substr(2));
				glm::vec3 vertex;
				v >> vertex.x; v >> vertex.y; v >> vertex.z;
				tempVerticies.push_back(vertex);
				//std::cout << "V : " <<vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
			}
			else if (lineBuffer.substr(0,2) == "vt") {
				std::istringstream vt(lineBuffer.substr(3));
				glm::vec2 uv; // ignore w
				vt >> uv.s; vt >> uv.t;
				tempUVs.push_back(uv);
				//std::cout << "UV : " << uv.x << " " << uv.y << std::endl;
			}
			else if (lineBuffer.substr(0,2) == "f ") {

				int p1, p2, p3; // store mesh index
				int t1, t2, t3; // store texture index
				int n1, n2, n3; // store normal index
				const char* face = lineBuffer.c_str();
				//only loads triangles
				int match = sscanf_s(face, "f %i/%i/%i %i/%i/%i %i/%i/%i",
					&p1, &t1, &n1,
					&p2, &t2, &n2,
					&p3, &t3, &n3);

				//std::cout << "Layout : " << p1 << " " << t1 << " " << n1 << std::endl;
				//std::cout << "Layout : " << p2 << " " << t2 << " " << n2 << std::endl;
				//std::cout << "Layout : " << p3 << " " << t3 << " " << n3 << std::endl;

				if (match != 9){
					std::cout << "Failed to parse .OBJ due to it having too much data for this read" << std::endl;
				}

				//ignore normals till lighting 

				vertexIndicies.push_back(p1);
				vertexIndicies.push_back(p2);
				vertexIndicies.push_back(p3);

				uvIndicies.push_back(t1);
				uvIndicies.push_back(t2);
				uvIndicies.push_back(t3);
			}
		}

		//close file
		fin.close();

		for (unsigned int i = 0; i < vertexIndicies.size(); i++) {
			glm::vec3 vertex = tempVerticies[vertexIndicies[i] - 1];
			glm::vec2 uv = tempUVs[uvIndicies[i] - 1];

			Vertex meshVertex;
			meshVertex.position = vertex;
			meshVertex.texCoords = uv;

			mVerticies.push_back(meshVertex);
		}

		InitBuffers();

		return (mLoaded = true);
	}

	return false;
}

void Mesh::Draw() {
	if (!mLoaded) { return; }

	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, mVerticies.size());
	glBindVertexArray(0);
}

void Mesh::InitBuffers() {
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mVerticies.size() * sizeof(Vertex), &mVerticies[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	// position attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(0);

	// text Coord 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}
