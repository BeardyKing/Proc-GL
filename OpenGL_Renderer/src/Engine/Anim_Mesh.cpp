#include "Anim_Mesh.h"

using std::string;
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream

Anim_Mesh::Anim_Mesh(void) {
	glGenVertexArrays(1, &arrayObject);

	for (int i = 0; i < MAX_BUFFER; ++i) {
		bufferObject[i] = 0;
	}

	numVertices = 0;
	type = GL_TRIANGLES;

	numIndices = 0;
	indices = nullptr;
	vertices = nullptr;
	textureCoords = nullptr;
	normals = nullptr;
	tangents = nullptr;
	indices = nullptr;
	colours = nullptr;
	weights = nullptr;
	weightIndices = nullptr;
}

Anim_Mesh::~Anim_Mesh(void) {
	glDeleteVertexArrays(1, &arrayObject);			//Delete our VAO
	glDeleteBuffers(MAX_BUFFER, bufferObject);		//Delete our VBOs

	delete[]	vertices;
	delete[]	indices;
	delete[]	textureCoords;
	delete[]	tangents;
	delete[]	normals;
	delete[]	colours;
	delete[]	weights;
	delete[]	weightIndices;
}

void Anim_Mesh::Draw() {
	glBindVertexArray(arrayObject);
	if (bufferObject[INDEX_BUFFER]) {
		glDrawElements(type, numIndices, GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(type, 0, numVertices);
	}
	glBindVertexArray(0);
}

void Anim_Mesh::DrawSubMesh(int i) {
	if (i < 0 || i >= (int)meshLayers.size()) {
		return;
	}
	SubMesh m = meshLayers[i];

	glBindVertexArray(arrayObject);
	if (bufferObject[INDEX_BUFFER]) {
		const GLvoid* offset = (const GLvoid*)(m.start * sizeof(unsigned int));
		glDrawElements(type, m.count, GL_UNSIGNED_INT, offset);
	}
	else {
		glDrawArrays(type, m.start, m.count);	//Draw the triangle!
	}
	glBindVertexArray(0);
}

void UploadAttribute(GLuint* id, int numElements, int dataSize, int attribSize, int attribID, void* pointer, const string& debugName) {
	glGenBuffers(1, id);
	glBindBuffer(GL_ARRAY_BUFFER, *id);
	glBufferData(GL_ARRAY_BUFFER, numElements * dataSize, pointer, GL_STATIC_DRAW);

	glVertexAttribPointer(attribID, attribSize, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attribID);

	glObjectLabel(GL_BUFFER, *id, -1, debugName.c_str());
}

void	Anim_Mesh::BufferData() {
	glBindVertexArray(arrayObject);

	////Buffer vertex data
	UploadAttribute(&bufferObject[VERTEX_BUFFER], numVertices, sizeof(glm::vec3), 3, VERTEX_BUFFER, vertices, "Positions");

	if (textureCoords) {	//Buffer texture data
		UploadAttribute(&bufferObject[TEXTURE_BUFFER], numVertices, sizeof(glm::vec2), 2, TEXTURE_BUFFER, textureCoords, "TexCoords");
	}

	if (colours) {
		UploadAttribute(&bufferObject[COLOUR_BUFFER], numVertices, sizeof(glm::vec4), 4, COLOUR_BUFFER, colours, "Colours");
	}

	if (normals) {	//Buffer normal data
		UploadAttribute(&bufferObject[NORMAL_BUFFER], numVertices, sizeof(glm::vec3), 3, NORMAL_BUFFER, normals, "Normals");
	}

	if (tangents) {	//Buffer tangent data
		UploadAttribute(&bufferObject[TANGENT_BUFFER], numVertices, sizeof(glm::vec4), 4, TANGENT_BUFFER, tangents, "Tangents");
	}

	if (weights) {		//Buffer weights data
		UploadAttribute(&bufferObject[WEIGHTVALUE_BUFFER], numVertices, sizeof(glm::vec4), 4, WEIGHTVALUE_BUFFER, weights, "Weights");
	}

	//Buffer weight indices data...uses a different function since its integers...
	if (weightIndices) {
		glGenBuffers(1, &bufferObject[WEIGHTINDEX_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[WEIGHTINDEX_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(int) * 4, weightIndices, GL_STATIC_DRAW);
		glVertexAttribIPointer(WEIGHTINDEX_BUFFER, 4, GL_INT, 0, 0); //note the new function...
		glEnableVertexAttribArray(WEIGHTINDEX_BUFFER);

		glObjectLabel(GL_BUFFER, bufferObject[WEIGHTINDEX_BUFFER], -1, "Weight Indices");
	}

	//buffer index data
	//if(indices) {
	//	glGenBuffers(1, &bufferObject[INDEX_BUFFER]);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObject[INDEX_BUFFER]);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices*sizeof(GLuint), indices, GL_STATIC_DRAW);
	//
	//	glObjectLabel(GL_BUFFER, bufferObject[INDEX_BUFFER], -1, "Indices");
	//}
	if (indices) {
		glGenBuffers(1, &bufferObject[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObject[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint),
			indices, GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


/*
*
* Extra file loading stuff!
*
* */

enum class GeometryChunkTypes {
	VPositions = 1,
	VNormals = 2,
	VTangents = 4,
	VColors = 8,
	VTex0 = 16,
	VTex1 = 32,
	VWeightValues = 64,
	VWeightIndices = 128,
	Indices = 256,
	JointNames = 512,
	JointParents = 1024,
	BindPose = 2048,
	BindPoseInv = 4096,
	Material = 65536,
	SubMeshes = 1 << 14,
	SubMeshNames = 1 << 15
};

void ReadTextFloats(std::ifstream& file, std::vector<glm::vec2>& element, int numVertices) {
	for (int i = 0; i < numVertices; ++i) {
		glm::vec2 temp;
		file >> temp.x;
		file >> temp.y;
		element.emplace_back(temp);
	}
}

void ReadTextFloats(std::ifstream& file, std::vector<glm::vec3>& element, int numVertices) {
	for (int i = 0; i < numVertices; ++i) {
		glm::vec3 temp;
		file >> temp.x;
		file >> temp.y;
		file >> temp.z;
		element.emplace_back(temp);
	}
}

void ReadTextFloats(std::ifstream& file, std::vector<glm::vec4>& element, int numVertices) {
	for (int i = 0; i < numVertices; ++i) {
		glm::vec4 temp;
		file >> temp.x;
		file >> temp.y;
		file >> temp.z;
		file >> temp.w;
		element.emplace_back(temp);
	}
}

void ReadTextVertexIndices(std::ifstream& file, std::vector<int>& element, int numVertices) {
	for (int i = 0; i < numVertices; ++i) {
		int indices[4];
		file >> indices[0];
		file >> indices[1];
		file >> indices[2];
		file >> indices[3];
		element.emplace_back(indices[0]);
		element.emplace_back(indices[1]);
		element.emplace_back(indices[2]);
		element.emplace_back(indices[3]);
	}
}

void ReadIndices(std::ifstream& file, std::vector<unsigned int>& elements, int numIndices) {
	for (int i = 0; i < numIndices; ++i) {
		unsigned int temp;
		file >> temp;
		elements.emplace_back(temp);
	}
}

void ReadJointParents(std::ifstream& file, std::vector<int>& dest) {
	int jointCount = 0;
	file >> jointCount;

	for (int i = 0; i < jointCount; ++i) {
		int id = -1;
		file >> id;
		dest.emplace_back(id);
	}
}

void ReadJointNames(std::ifstream& file, std::vector<std::string>& dest) {
	int jointCount = 0;
	file >> jointCount;
	for (int i = 0; i < jointCount; ++i) {
		std::string jointName;
		file >> jointName;
		dest.emplace_back(jointName);
	}
}

void ReadRigPose(std::ifstream& file, glm::mat4** into) {
	int matCount = 0;
	file >> matCount;

	*into = new glm::mat4[matCount];

	for (int i = 0; i < matCount; ++i) {
		glm::mat4 mat;
		float arr[16]{};
		for (int i = 0; i < 16; ++i) {
			file >> arr[i];
		}
		mat = glm::make_mat4(arr);
		(*into)[i] = mat;
	}
}

void ReadSubMeshes(std::ifstream& file, int count, std::vector<Anim_Mesh::SubMesh>& subMeshes) {
	for (int i = 0; i < count; ++i) {
		Anim_Mesh::SubMesh m;
		file >> m.start;
		file >> m.count;
		subMeshes.emplace_back(m);
	}
}

void ReadSubMeshNames(std::ifstream& file, int count, std::vector<std::string>& names) {
	std::string scrap;
	std::getline(file, scrap);

	for (int i = 0; i < count; ++i) {
		std::string meshName;
		std::getline(file, meshName);
		names.emplace_back(meshName);
	}
}


Anim_Mesh* Anim_Mesh::GenerateTriangle()
{
	Anim_Mesh* m = new  Anim_Mesh();
	m->numVertices = 3;
	m->type = GL_TRIANGLES;

	m->vertices = new  glm::vec3[m->numVertices];
	m->vertices[0] = glm::vec3(0.0f, 0.5f, 0.0f);
	m->vertices[1] = glm::vec3(0.5f, -0.5f, 0.0f);
	m->vertices[2] = glm::vec3(-0.5f, -0.5f, 0.0f);

	m->colours = new	glm::vec4[m->numVertices];
	m->colours[0] =		glm::vec4(rand(), 0.0f, 0.0f, 1.0f);
	m->colours[1] =		glm::vec4(rand(), 1.0f, 0.0f, 1.0f);
	m->colours[2] =		glm::vec4(rand(), 0.0f, 1.0f, 1.0f);

	m->textureCoords = new	glm::vec2[m->numVertices];
	m->textureCoords[0] =	glm::vec2(0.5f, 0.0f);
	m->textureCoords[1] =	glm::vec2(1.0f, 1.0f);
	m->textureCoords[2] =	glm::vec2(0.0f, 1.0f);


	m->BufferData();
	return m;
}

Anim_Mesh* Anim_Mesh::GenerateQuad() {
	Anim_Mesh* m = new Anim_Mesh();
	m->numVertices = 4;
	m->type = GL_TRIANGLE_STRIP;

	m->vertices = new		glm::vec3[m->numVertices];
	m->textureCoords = new	glm::vec2[m->numVertices];
	m->colours = new		glm::vec4[m->numVertices];

	m->vertices[0] = glm::vec3(-1.0f, 1.0f, 0.0f);
	m->vertices[1] = glm::vec3(-1.0f, -1.0f, 0.0f);
	m->vertices[2] = glm::vec3(1.0f, 1.0f, 0.0f);
	m->vertices[3] = glm::vec3(1.0f, -1.0f, 0.0f);

	m->textureCoords[0] = glm::vec2(0.0f, 1.0f);
	m->textureCoords[1] = glm::vec2(0.0f, 0.0f);
	m->textureCoords[2] = glm::vec2(1.0f, 1.0f);
	m->textureCoords[3] = glm::vec2(1.0f, 0.0f);

	for (int i = 0; i < 4; ++i) {
		m->colours[i] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	m->normals = new glm::vec3[m->numVertices]; // Init new var !
	m->tangents = new glm::vec4[m->numVertices]; // Init new var !

	for (int i = 0; i < 4; ++i) {
		m->colours[i] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m->normals[i] = glm::vec3(0.0f, 0.0f, -1.0f); // New !
		m->tangents[i] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // New !
	}

	m->BufferData();
	return m;
}

Anim_Mesh* Anim_Mesh::LoadFromMeshFile(const string& name) {
	Anim_Mesh* mesh = new Anim_Mesh();

	std::ifstream file(name);

	std::string filetype;
	int fileVersion;

	file >> filetype;

	if (filetype != "MeshGeometry") {
		std::cout << "File is not a MeshGeometry file!" << std::endl;
		return nullptr;
	}

	file >> fileVersion;

	if (fileVersion != 1) {
		std::cout << "MeshGeometry file has incompatible version!" << std::endl;
		return nullptr;
	}

	int numMeshes = 0; //read
	int numVertices = 0; //read
	int numIndices = 0; //read
	int numChunks = 0; //read

	file >> numMeshes;
	file >> numVertices;
	file >> numIndices;
	file >> numChunks;

	std::vector<glm::vec3> readPositions;
	std::vector<glm::vec4> readColours;
	std::vector<glm::vec3> readNormals;
	std::vector<glm::vec4> readTangents;
	std::vector<glm::vec2> readUVs;
	std::vector<glm::vec4> readWeights;
	std::vector<int> readWeightIndices;

	std::vector<unsigned int>		readIndices;

	for (int i = 0; i < numChunks; ++i) {
		int chunkType = (int)GeometryChunkTypes::VPositions;

		file >> chunkType;

		switch ((GeometryChunkTypes)chunkType) {
		case GeometryChunkTypes::VPositions:ReadTextFloats(file, readPositions, numVertices);  break;
		case GeometryChunkTypes::VColors:	ReadTextFloats(file, readColours, numVertices);  break;
		case GeometryChunkTypes::VNormals:	ReadTextFloats(file, readNormals, numVertices);  break;
		case GeometryChunkTypes::VTangents:	ReadTextFloats(file, readTangents, numVertices);  break;
		case GeometryChunkTypes::VTex0:		ReadTextFloats(file, readUVs, numVertices);  break;
		case GeometryChunkTypes::Indices:	ReadIndices(file, readIndices, numIndices); break;

		case GeometryChunkTypes::VWeightValues:		ReadTextFloats(file, readWeights, numVertices);  break;
		case GeometryChunkTypes::VWeightIndices:	ReadTextVertexIndices(file, readWeightIndices, numVertices);  break;
		case GeometryChunkTypes::JointNames:		ReadJointNames(file, mesh->jointNames);  break;
		case GeometryChunkTypes::JointParents:		ReadJointParents(file, mesh->jointParents);  break;
		case GeometryChunkTypes::BindPose:			ReadRigPose(file, &mesh->bindPose);  break;
		case GeometryChunkTypes::BindPoseInv:		ReadRigPose(file, &mesh->inverseBindPose);  break;
		case GeometryChunkTypes::SubMeshes: 		ReadSubMeshes(file, numMeshes, mesh->meshLayers); break;
		case GeometryChunkTypes::SubMeshNames: 		ReadSubMeshNames(file, numMeshes, mesh->layerNames); break;
		}
	}
	//Now that the data has been read, we can shove it into the actual Mesh object

	mesh->numVertices = numVertices;
	mesh->numIndices = numIndices;

	if (!readPositions.empty()) {
		mesh->vertices = new glm::vec3[numVertices];
		memcpy(mesh->vertices, readPositions.data(), numVertices * sizeof(glm::vec3));
	}

	if (!readColours.empty()) {
		mesh->colours = new glm::vec4[numVertices];
		memcpy(mesh->colours, readColours.data(), numVertices * sizeof(glm::vec4));
	}

	if (!readNormals.empty()) {
		mesh->normals = new glm::vec3[numVertices];
		memcpy(mesh->normals, readNormals.data(), numVertices * sizeof(glm::vec3));
	}

	if (!readTangents.empty()) {
		mesh->tangents = new glm::vec4[numVertices];
		memcpy(mesh->tangents, readTangents.data(), numVertices * sizeof(glm::vec4));
	}

	if (!readUVs.empty()) {
		mesh->textureCoords = new glm::vec2[numVertices];
		memcpy(mesh->textureCoords, readUVs.data(), numVertices * sizeof(glm::vec2));
	}
	if (!readIndices.empty()) {
		mesh->indices = new unsigned int[numIndices];
		memcpy(mesh->indices, readIndices.data(), numIndices * sizeof(unsigned int));
	}

	if (!readWeights.empty()) {
		mesh->weights = new glm::vec4[numVertices];
		memcpy(mesh->weights, readWeights.data(), numVertices * sizeof(glm::vec4));
	}

	if (!readWeightIndices.empty()) {
		mesh->weightIndices = new int[numVertices * 4];
		memcpy(mesh->weightIndices, readWeightIndices.data(), numVertices * sizeof(int) * 4);
	}

	mesh->BufferData();

	return mesh;
}

int Anim_Mesh::GetIndexForJoint(const std::string& name) const {
	for (unsigned int i = 0; i < jointNames.size(); ++i) {
		if (jointNames[i] == name) {
			return i;
		}
	}
	return -1;
}

int Anim_Mesh::GetParentForJoint(const std::string& name) const {
	int i = GetIndexForJoint(name);
	if (i == -1) {
		return -1;
	}
	return jointParents[i];
}

int Anim_Mesh::GetParentForJoint(int i) const {
	if (i == -1 || i >= (int)jointParents.size()) {
		return -1;
	}
	return jointParents[i];
}

bool Anim_Mesh::GetSubMesh(int i, const SubMesh* s) const {
	if (i < 0 || i >= (int)meshLayers.size()) {
		return false;
	}
	s = &meshLayers[i];
	return true;
}

bool Anim_Mesh::GetSubMesh(const string& name, const SubMesh* s) const {
	for (unsigned int i = 0; i < layerNames.size(); ++i) {
		if (layerNames[i] == name) {
			return GetSubMesh(i, s);
		}
	}
	return false;
}

bool Anim_Mesh::GetVertexIndicesForTri(unsigned int i, unsigned int& a, unsigned int& b, unsigned int& c) const {
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

void Anim_Mesh::GenerateNormals() {
	if (!normals) {
		normals = new glm::vec3[numVertices];
	}
	for (GLuint i = 0; i < numVertices; ++i) {
		normals[i] = glm::vec3();
	}

	int triCount = GetTriCount();
	for (int i = 0; i < triCount; ++i) {
		unsigned int a = 0;
		unsigned int b = 0;
		unsigned int c = 0;
		GetVertexIndicesForTri(i, a, b, c);

		glm::vec3 normal = glm::cross(glm::vec3(vertices[b] - vertices[a]), glm::vec3(vertices[c] - vertices[a]));

		normals[a] += normal;
		normals[b] += normal;
		normals[c] += normal;
	}
	for (GLuint i = 0; i < numVertices; ++i) {
		glm::normalize(normals[i]);
	}
}

void Anim_Mesh::GenerateTangents() {
	if (!textureCoords) {
		return;
	}
	if (!tangents) {
		tangents = new glm::vec4[numVertices];
	}
	for (GLuint i = 0; i < numVertices; ++i) {
		tangents[i] = glm::vec4(0, 0, 0, 0);
	}

	int triCount = GetTriCount();

	for (int i = 0; i < triCount; ++i) {
		unsigned int a = 0;
		unsigned int b = 0;
		unsigned int c = 0;
		GetVertexIndicesForTri(i, a, b, c);
		glm::vec4 tangent = GenerateTangent(a, b, c);
		tangents[a] += tangent;
		tangents[b] += tangent;
		tangents[c] += tangent;
	}
	for (GLuint i = 0; i < numVertices; ++i) {
		float handedness = tangents[i].w > 0.0f ? 1.0f : -1.0f;
		tangents[i].w = 0.0f;
		glm::normalize(tangents[i]);
		tangents[i].w = handedness;
	}
}

glm::vec4 Anim_Mesh::GenerateTangent(int a, int b, int c) {
	glm::vec3 ba = vertices[b] - vertices[a];
	glm::vec3 ca = vertices[c] - vertices[a];

	glm::vec2 tba = textureCoords[b] - textureCoords[a];
	glm::vec2 tca = textureCoords[c] - textureCoords[a];

	//Matrix2 texMatrix = Matrix2(tba, tca);
	glm::mat2 texMatrix = glm::mat2(tba, tca);
	texMatrix = glm::inverse(texMatrix);
	//texMatrix.Invert();

	glm::vec3 tangent;
	glm::vec3 binormal;

	//tangent = ba * texMatrix.values[0] + ca * texMatrix.values[1]; // TO DO CHECK CORRECT
	//binormal = ba * texMatrix.values[2] + ca * texMatrix.values[3];

	tangent = ba * texMatrix[0][0] + ca * texMatrix[1][0];
	binormal = ba * texMatrix[0][1] + ca * texMatrix[1][1];

	glm::vec3  normal = glm::cross(ba, ca);
	glm::vec3  biCross = glm::cross(tangent, normal);

	float handedness = 1.0f;
	//if (Vector3::Dot(biCross, binormal) < 0.0f) {
	if (glm::dot(biCross, binormal) < 0.0f) {
		handedness = -1.0f;
	}

	return glm::vec4(tangent.x, tangent.y, tangent.z, handedness);
}

