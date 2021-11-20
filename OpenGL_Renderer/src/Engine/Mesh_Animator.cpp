
#include "Mesh_Animator.h"

//#include "Matrix4.h"

#include <fstream>
#include <string>

MeshAnimation::MeshAnimation() {
	jointCount = 0;
	frameCount = 0;
	frameRate = 0.0f;
}

MeshAnimation::MeshAnimation(const std::string& filename) : MeshAnimation() {
	std::ifstream file(filename);

	std::string filetype;
	int fileVersion;

	file >> filetype;

	if (filetype != "MeshAnim") {
		std::cout << "File is not a MeshAnim file!" << std::endl;
		return;
	}
	file >> fileVersion;
	file >> frameCount;
	file >> jointCount;
	file >> frameRate;

	allJoints.reserve(frameCount * jointCount);

	for (unsigned int f = 0; f < frameCount; ++f) {
		for (unsigned int j = 0; j < jointCount; ++j) {
			glm::mat4 mat;
			float arr[16]; // TODO CHECK THIS IS FORMATTING THE DATA CORRECTLY

			for (int i = 0; i < 16; ++i) {
				file >> arr[i];
			}
			mat = glm::make_mat4(arr);
			allJoints.emplace_back(mat);
		}
	}
}

MeshAnimation::~MeshAnimation() {

}

const glm::mat4* MeshAnimation::GetJointData(unsigned int frame) const {
	if (frame >= frameCount) {
		return nullptr;
	}
	int matStart = frame * jointCount;

	glm::mat4* dataStart = (glm::mat4*)allJoints.data();

	return dataStart + matStart;
}

void MeshAnimation::OnUpdate(double deltaTime)
{
}


void MeshAnimation::OnRender()
{
}

void MeshAnimation::OnImGuiRender() {
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

	ImGui::Begin("Inspector"); {
		ImGui::Separator();

		if (ImGui::CollapsingHeader("MESH_ANIMATION", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
			ImGui::Indent();
			ImGui::Text("MESH_ANIMATION DATA");

			ImGui::Unindent();
		}
		ImGui::Separator();
	}
	ImGui::End();
}