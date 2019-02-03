#pragma once
#include <string>
#include <map>
#include <vector>

class SaveData {
public:
	SaveData(std::string name);
	SaveData(std::string name, std::map<std::string, std::string> values);
	// Add a string value to the data
	void addValue(std::string key, std::string value);
	// Add a SaveData nested in this save data
	void addData(SaveData data);
	// Get a string value
	std::string getValue(std::string key);
	// Get the values
	std::map<std::string, std::string> getValues();
	// Get the Save Datas
	std::vector<SaveData> getDatas();
	// Get the string name
	std::string getName();
private:
	// Map of string values
	std::map<std::string, std::string> values;
	// Vector of savedatas
	std::vector<SaveData> datas;
	// Name
	std::string name;
};
