#ifndef LIGHTOBJECT_H
#define LIGHTOBJECT_H

#include "ShaderProgram.h"
#include "Mesh.h"
#include "glm/glm.hpp"

#include <memory>



class LightObject
{
public:
	LightObject();
	~LightObject();

	void LoadMesh	(const char* meshPath);
	void LoadShader	(const char* vertPath, const char* fragPath);



private:

private:

	const char* m_vertexShaderPath;
	const char* m_fragmentShaderPath;
	const char* m_meshPath;


public:
	std::unique_ptr <Mesh> m_Mesh;
	std::unique_ptr <ShaderProgram> m_Shader;

	enum LightType{ Point, Directional, Spot};
	LightType lightType;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::vec3 color;
	glm::vec3 direction;
};

#endif // !LIGHTOBJECT_H


