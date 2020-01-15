#include "SaveData.h"

SaveData::SaveData(std::string name) {
	this->name = name;
}
SaveData::SaveData(std::string name, std::map<std::string, std::string> values) {
	this->name = name;
	this->values = values;
}
void SaveData::addValue(std::string key, std::string value) {
	this->values[key] = value;
}
void SaveData::addData(SaveData data) {
	this->datas.push_back(data);
}
std::string SaveData::getValue(std::string key) {
	return this->values[key];
}
std::map<std::string, std::string> SaveData::getValues() {
	return this->values;
}
std::vector<SaveData> SaveData::getDatas() {
	return this->datas;
}
std::string SaveData::getName() {
	return this->name;
}