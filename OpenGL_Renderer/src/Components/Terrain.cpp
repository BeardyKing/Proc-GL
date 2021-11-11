#include "Terrain.h"
//#include "Terrain.h"

#include "stb_image/stb_image.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec4 tangents;
};

Terrain::Terrain()
{
	std::string fileName = "island_test_3.png";

	unsigned char* data;
	int iWidth, iHeight, iChans;
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(fileName.c_str(), &iWidth, &iHeight, &iChans, 1);


	if (!data) {
		std::cout << " Heightmap can ’t load file !\n";
		return;
	}

	numVertices = iWidth * iHeight;
	numIndices = (iWidth - 1) * (iHeight - 1) * 6;
	vertices.reserve(numVertices);
	textureCoords.reserve(numVertices);
	indices = new GLuint[numIndices];
	glm::vec3 vertexScale = glm::vec3(16.0f, 5.0f, 16.0f); // value not interpolating seeing steps instead of gradient
	glm::vec2 textureScale = glm::vec2(1 / 16.0f, 1 / 16.0f);

	for (int z = 0; z < iHeight; ++z) {
		for (int x = 0; x < iWidth; ++x) {
			int offset = (z * iWidth) + x;
			vertices[offset] = glm::vec3(x, data[offset], z) * vertexScale;
			textureCoords[offset] = glm::vec2(x, z) * textureScale;
		}
	}
	stbi_image_free(data);

	int i = 0;

	for (int z = 0; z < iHeight - 1; ++z) {
		for (int x = 0; x < iWidth - 1; ++x) {
			int a = (z * (iWidth)) + x;
			int b = (z * (iWidth)) + (x + 1);
			int c = ((z + 1) * (iWidth)) + (x + 1);
			int d = ((z + 1) * (iWidth)) + x;

			indices[i++] = a;
			indices[i++] = c;
			indices[i++] = b;

			indices[i++] = c;
			indices[i++] = a;
			indices[i++] = d;
		}
	}
	// Gen normals
	
	if (normals.empty()) {
		normals.reserve(numVertices);
	}
	for (GLuint i = 0; i < numVertices; ++i) {
		normals.emplace_back(glm::vec3());
	}

	int triCount = GetTriCount();

	for (int i = 0; i < triCount; ++i) {
		unsigned int a = 0;
		unsigned int b = 0;
		unsigned int c = 0;
		GetVertexIndicesForTri(i, a, b, c);

		glm::vec3 normal = glm::cross((vertices[b] - vertices[a]), (vertices[c] - vertices[a]));
		normals[a] += normal;
		normals[b] += normal;
		normals[c] += normal;
	}
	for (GLuint i = 0; i < numVertices; ++i) {
		glm::normalize(normals[i]);
	}

	Vertex temp;
	for (size_t i = 0; i < numVertices; i++){
		temp.position = vertices[i];
		temp.normal = normals[i];
		temp.texCoords = textureCoords[i];

		mVertexData.vData.emplace_back(temp);
	}
	for (size_t i = 0; i < numIndices; i++){
		mVertexData.indices.emplace_back(indices[i]);
	}
}

Terrain::~Terrain(){
	delete[] indices;
}

std::vector<Vertex> Terrain::GetVertices() {
	return mVertices;
}

unsigned int Terrain::GetTriCount() const {
	int primCount = indices ? numIndices : numVertices;
	return primCount / 3;
}

VertexData Terrain::GetVertexData(){
	return mVertexData;
}

bool Terrain::GetVertexIndicesForTri(unsigned int i, unsigned int& a, unsigned int& b, unsigned int& c) const {
	unsigned int triCount = GetTriCount();

	if (i >= triCount) {
		return false;
	}
	if (numIndices > 0) {
		a = indices[(i * 3)];
		b = indices[(i * 3) + 1];
		c = indices[(i * 3) + 2];
	}
	else {
		a = (i * 3);
		b = (i * 3) + 1;
		c = (i * 3) + 2;
	}
	return true;
}
