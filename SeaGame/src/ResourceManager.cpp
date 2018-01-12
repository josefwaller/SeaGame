#include "ResourceManager.h"
#include <rapidxml\rapidxml.hpp>
#include <rapidxml\rapidxml_utils.hpp>

ResourceManager::ResourceManager()
{

}

void ResourceManager::ensureSheetIsLoaded(std::string sheetName)
{
	if (this->sheets.find(sheetName) == this->sheets.end()) {
		loadSpritesheet(sheetName);
	}
}
std::pair<const sf::Texture*, sf::IntRect> ResourceManager::getSpriteInfo(std::string sheetName, std::string spriteName)
{
	ensureSheetIsLoaded(sheetName);
	std::pair<sf::Texture, std::map<std::string, sf::IntRect>>* sheet = &this->sheets[sheetName];

	return { &sheet->first, sheet->second[spriteName] };
}
void ResourceManager::loadSpritesheet(std::string sheetName)
{
	// Load the document
	rapidxml::xml_document<> doc;
	rapidxml::file<> file(std::string("assets/sprites/" + sheetName + ".xml").c_str());
	doc.parse<0>(file.data());
	// Get the TextureAtlas
	rapidxml::xml_node<> *atlas = doc.first_node("TextureAtlas");
	// Get the name of the image
	std::string imageName = atlas->first_attribute("imagePath")->value();
	// Load the texture
	// Note that it uses a reference of the texture in the map, so that the
	//    original texture does not go out of scope
	this->sheets.insert({ sheetName, { sf::Texture(), {}} });
	sf::Texture* t = &this->sheets[sheetName].first;
	t->loadFromFile("assets/sprites/" + imageName);
	// Store texture in map
	// Get the first SubTexture
	rapidxml::xml_node<> *sub = atlas->first_node("SubTexture");
	while (sub != nullptr)
	{
		// Get the area of the image that this sprite uses
		sf::IntRect r(
			std::stoi(sub->first_attribute("x")->value()),
			std::stoi(sub->first_attribute("y")->value()),
			std::stoi(sub->first_attribute("width")->value()),
			std::stoi(sub->first_attribute("height")->value())
		);
		// Store it
		this->sheets[sheetName].second.insert({ std::string(sub->first_attribute("name")->value()), r });
		// Go to next subtexture
		sub = sub->next_sibling();
	}
}

ResourceManager* ResourceManager::get()
{
	static ResourceManager r;
	return &r;
}