#ifndef MESHMATERIAL_H
#define MESHMATERIAL_H

#include <string>
#include <map>
#include <vector>

#include "../ECS/Engine_ECS.h"

using std::map;
using std::string;
using std::vector;

class MeshMaterialEntry {
public:
	std::map<string, string> entries;

	bool GetEntry(const string& name, const string** output) const {
		auto i = entries.find(name);
		if (i == entries.end()) {
			return false;
		}
		*output = &i->second;
		return true;
	}

};

class MeshMaterial : public Component
{
public:

	MeshMaterial(const std::string& filename);
	~MeshMaterial() {}
	const MeshMaterialEntry* GetMaterialForLayer(int i) const;
	
	void OnUpdate(double deltaTime);
	void OnRender();
	void OnImGuiRender();

protected:
	std::vector<MeshMaterialEntry>	materialLayers;
	std::vector<MeshMaterialEntry*> meshLayers;
};

#endif

