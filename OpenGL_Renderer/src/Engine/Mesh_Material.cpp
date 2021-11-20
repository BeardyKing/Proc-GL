
#include "Mesh_Material.h"

#include <fstream>
#include <iostream>

using std::ifstream;

MeshMaterial::MeshMaterial(const std::string& filename) {
	ifstream file(filename);

	string dataType;
	file >> dataType;

	if (dataType != "MeshMat") {
		std::cout << "File " << filename << " is not a MeshMaterial!\n";
		return;
	}
	int version;
	file >> version;

	if (version != 1) {
		std::cout << "File " << filename << " has incompatible version " << version << "!\n";
		return;
	}

	int matCount;
	int meshCount;
	file >> matCount;
	file >> meshCount;

	materialLayers.resize(matCount);

	for (int i = 0; i < matCount; ++i) {
		string name;
		int count;
		file >> name;
		file >> count;

		for (int j = 0; j < count; ++j) {
			string entryData;
			file >> entryData;
			string channel;
			string file;
			size_t split = entryData.find_first_of(':');
			channel = entryData.substr(0, split);
			file = entryData.substr(split + 1);

			materialLayers[i].entries.insert(std::make_pair(channel, file));
		}
	}

	for (int i = 0; i < meshCount; ++i) {
		int entry;
		file >> entry;
		meshLayers.emplace_back(&materialLayers[entry]);
	}
}

const MeshMaterialEntry* MeshMaterial::GetMaterialForLayer(int i) const {
	if (i < 0 || i >= (int)meshLayers.size()) {
		return nullptr;
	}
	return meshLayers[i];
}

void MeshMaterial::OnUpdate(double deltaTime)
{
}


void MeshMaterial::OnRender()
{
}

void MeshMaterial::OnImGuiRender() {
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

	ImGui::Begin("Inspector"); {
		ImGui::Separator();

		if (ImGui::CollapsingHeader("MESH_MATERIALS", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
			ImGui::Indent();
			ImGui::Text("MESH_MATERIALS DATA");

			ImGui::Unindent();
		}
		ImGui::Separator();
	}
	ImGui::End();
}
