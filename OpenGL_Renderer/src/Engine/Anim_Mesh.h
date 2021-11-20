/******************************************************************************
Class:Mesh
Implements:
Author:Rich Davison	 <richard-gordon.davison@newcastle.ac.uk>
Description:Wrapper around OpenGL primitives, geometry and related
OGL functions.

There's a couple of extra functions in here that you didn't get in the tutorial
series, to draw debug normals and tangents.


-_-_-_-_-_-_-_,------,
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""

*//////////////////////////////////////////////////////////////////////////////

#ifndef ANIM_MESH_H 
#define ANIM_MESH_H


//#include "OGLRenderer.h"

#include <vector>
#include <string>

#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "../ECS/Engine_ECS.h"

//A handy enumerator, to determine which member of the bufferObject array
//holds which data

enum MeshBuffer {
	VERTEX_BUFFER,
	COLOUR_BUFFER,
	TEXTURE_BUFFER,
	NORMAL_BUFFER,
	TANGENT_BUFFER,

	WEIGHTVALUE_BUFFER,		//new this year, weight values of vertices
	WEIGHTINDEX_BUFFER,	//new this year, indices of weights

	INDEX_BUFFER,

	MAX_BUFFER
};

class Anim_Mesh {
public:
	struct SubMesh {
		int start;
		int count;
	};

	Anim_Mesh(void);
	~Anim_Mesh(void);

	void Draw();
	void DrawSubMesh(int i);

	static Anim_Mesh* GenerateQuad();
	static Anim_Mesh* GenerateTriangle();

	static Anim_Mesh* LoadFromMeshFile(const std::string& name);

	unsigned int GetTriCount() const {
		int primCount = indices ? numIndices : numVertices;
		return primCount / 3;
	}

	void GenerateNormals();
	bool GetVertexIndicesForTri(unsigned int i, unsigned int& a, unsigned int& b, unsigned int& c) const;


	int GetIndexForJoint(const std::string& name) const;
	int GetParentForJoint(const std::string& name) const;
	int GetParentForJoint(int i) const;

	unsigned int GetJointCount() const {
		return (unsigned int)jointNames.size();
	}

	const glm::mat4* GetBindPose() const {
		return bindPose;
	}

	const glm::mat4* GetInverseBindPose() const {
		return inverseBindPose;
	}


	int		GetSubMeshCount() const {
		return (int)meshLayers.size();
	}

	bool GetSubMesh(int i, const SubMesh* s) const;
	bool GetSubMesh(const std::string& name, const SubMesh* s) const;

protected:
	void	BufferData();

	GLuint	arrayObject;

	GLuint	bufferObject[MAX_BUFFER];

	GLuint	numVertices;
	GLuint numIndices;
	unsigned int* indices;

	GLuint	type;

	glm::vec3* vertices;
	glm::vec4* colours;
	glm::vec2* textureCoords;
	glm::vec3* normals;
	glm::vec4* tangents;

	glm::vec4* weights;
	int* weightIndices;

	void GenerateTangents();
	glm::vec4 GenerateTangent(int a, int b, int c);


	glm::mat4* bindPose;
	glm::mat4* inverseBindPose;

	std::vector<std::string>	jointNames;
	std::vector<int>			jointParents;
	std::vector< SubMesh>		meshLayers;
	std::vector<std::string>	layerNames;
};

#endif

