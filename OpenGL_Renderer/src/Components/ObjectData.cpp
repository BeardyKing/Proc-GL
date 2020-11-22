#include "ObjectData.h"
static uint32_t G_idCounter = 0;

uint32_t GetAmountOfEntities() {
	return G_idCounter;
}

ObjectData::ObjectData()
	:name("New Entity"){
	entity_id = G_idCounter;
	G_idCounter++;
}

ObjectData::ObjectData(char* _name)
	: name(name) {
	entity_id = G_idCounter;
	G_idCounter++;
}

ObjectData::ObjectData(std::string _name):
	name(_name){
	entity_id = G_idCounter;
	G_idCounter++;

}

ObjectData::~ObjectData(){
	
}

void ObjectData::OnExit() {
	std::cout << "RESET ENTITY COUNT" << std::endl;
	G_idCounter = 0;
}

void ObjectData::OnRender(){

}

void ObjectData::OnUpdate(double deltaTime){

}

void ObjectData::OnImGuiRender(){
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
	ImGui::Begin("Inspector");
	{

		char label[128] = "";
		sprintf_s(label, "ID %d", GetID());

		char* char_arr;
		std::string str_obj = GetName();
		char_arr = &str_obj[0];

		ImGui::InputText(label, char_arr, (int)(sizeof(char_arr) / sizeof(*(char_arr))));

		ImGui::Separator();

	}
	ImGui::End();
}

void ObjectData::SetName(char* _name){
	name = _name;
}

const char* ObjectData::GetName(){
	return name.c_str();
}


uint32_t ObjectData::GetID(){
	return entity_id;
}
