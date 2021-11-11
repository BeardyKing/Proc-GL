#ifndef TERRAIN_H
#define TERRAIN_H

#include "GL/glew.h"

#include "../ECS/Engine_ECS.h"

#include <string>
#include <vector>
#include <iostream>

//struct Vertex {
//	glm::vec3 position;
//	glm::vec3 normal;
//	glm::vec2 texCoords;
//};
struct Vertex;

struct VertexData {
	std::vector<Vertex> vData;
	std::vector<GLuint> indices;
};

class Terrain : public Component {
public:

	Terrain();
	~Terrain();
	//void OnImGuiRender();
	unsigned int GetTriCount() const;
	bool GetVertexIndicesForTri(unsigned int i, unsigned int& a, unsigned int& b, unsigned int& c) const;

	std::vector<Vertex> GetVertices();
	VertexData GetVertexData();

	VertexData mVertexData;
private:
	std::vector<Vertex> mVertices;


	GLuint	numVertices;
	GLuint numIndices;
	unsigned int* indices;

	GLuint	type;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec4> colours;
	std::vector<glm::vec2> textureCoords;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec4> tangents;

	std::vector<glm::vec4> weights;
	int* weightIndices;

	glm::vec3 heightmapSize;




};

#endif // ! TEXTURE2D_H
