#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

#include "GL/glew.h"
#include "glm/glm.hpp"

struct Vertex {
	glm::vec3 position;
	glm::vec2 texCoords;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool LoadOBJ(const std::string& fileName);
	void Draw();

private:
	void InitBuffers();

	bool mLoaded;
	std::vector<Vertex> mVerticies;
	GLuint mVBO, mVAO; // vertex buffer obj //vertex array obj

};

#endif

