#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "ECS/Component.h"
#include "ECS/Entity.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

class Mesh : public Component
{
public:
	Mesh();
	Mesh(const char* fileName);
	~Mesh();

	void OnRender();
	void OnUpdate();
	void OnImGuiRender();

	void OnExit();

	bool LoadOBJ(const std::string& fileName);
	void Draw();

private:
	void InitBuffers();

	bool mLoaded;
	std::vector<Vertex> mVertices;
	GLuint mVBO, mVAO; // vertex buffer obj //vertex array obj

};

#endif

