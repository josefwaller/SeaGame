#pragma once
#include <string>
#include <memory>
#include <rapidxml\rapidxml_utils.hpp>
#include "SaveData.h"

class App;
class Game;


class SaveFile {
public:
	// Get save data from an existing game
	SaveFile(Game* g);
	// Get the save data from a file
	SaveFile(std::string fileName);
	// Save the data to the file given, creating a file if necessary
	void save(std::string fileName);
	// Create a game from the data in the save file
	std::unique_ptr<Game> load(App* a);
private:
	// Underlying data structure
	std::string data;
	// Add the data in a SaveData to the rapidxml node given
	void addData(SaveData sd, rapidxml::xml_node<>* parent, rapidxml::xml_document<>* doc);
	// Load the data from xml to a SaveData
	SaveData getData(rapidxml::xml_node<>* node);
};
