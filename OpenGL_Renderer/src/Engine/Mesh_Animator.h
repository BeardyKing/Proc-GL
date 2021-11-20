#ifndef MESHANIMATION_H
#define MESHANIMATION_H

#include <vector>
#include <string>


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "../ECS/Engine_ECS.h"

class MeshAnimation : public Component
{
public:
	MeshAnimation();
	MeshAnimation(const std::string& filename);
	~MeshAnimation();

	void OnRender();
	void OnUpdate(double deltaTime);
	void OnImGuiRender();

	unsigned int GetJointCount() const {
		return jointCount;
	}

	unsigned int GetFrameCount() const {
		return frameCount;
	}

	float GetFrameRate() const {
		return frameRate;
	}

	const glm::mat4* GetJointData(unsigned int frame) const;

protected:
	unsigned int	jointCount;
	unsigned int	frameCount;
	float			frameRate;

	std::vector<glm::mat4>		allJoints;
};

#endif // MESHANIMATION_H
