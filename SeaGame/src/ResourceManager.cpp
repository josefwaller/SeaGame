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
std::map<std::string, LayoutSprite> ResourceManager::getLayout(std::string layoutName)
{
	auto it = this->layouts.find(layoutName);
	if (it != this->layouts.end()) {
		return it->second;
	}
	// Load xml file
	rapidxml::xml_document<> doc;
	rapidxml::file<> file(std::string("assets/sprites/layouts.xml").c_str());
	doc.parse<0>(file.data());
	// Get layout atlas
	rapidxml::xml_node<>* atlas = doc.first_node("LayoutAtlas");
	// Get first component node and iterate over others
	rapidxml::xml_node<>* layoutNode = atlas->first_node("Layout");
	while (layoutNode != nullptr) {
		// Get name
		std::string name = layoutNode->first_attribute("name")->value();
		// Insert new entry into map
		this->layouts.insert({ name, {} });
		// Get entry that was just inserted by reference
		std::map<std::string, LayoutSprite>* layout = &this->layouts[name];
		// Iterate over individual layout sprites
		rapidxml::xml_node<>* spriteNode = layoutNode->first_node("LayoutSprite");
		while (spriteNode != nullptr) {
			// Add sprite to layout
			layout->insert({
				spriteNode->first_attribute("name")->value(),
				{
					{
						std::stof(spriteNode->first_attribute("x")->value()),
						std::stof(spriteNode->first_attribute("y")->value())
					},
					std::stof(spriteNode->first_attribute("rot_deg")->value())
				}
			});

			spriteNode = spriteNode->next_sibling();
		}

		return this->layouts[layoutName];
	}

	return this->layouts[layoutName];
}
sf::Sprite ResourceManager::getSprite(std::string sheetName, std::string spriteName, bool center=false)
{
	auto spriteInfo = this->getSpriteInfo(sheetName, spriteName);
	sf::Sprite spr(*spriteInfo.first, spriteInfo.second);
	// If center parameter is given, set the sprite's origin to the center
	if (center) {
		spr.setOrigin({
			spr.getLocalBounds().width / 2,
			spr.getLocalBounds().height / 2
		});
	}
	return spr;
}
void ResourceManager::loadSpritesheet(std::string sheetName)
{
	rapidxml::xml_document<> doc;
	// Load the document
	rapidxml::file<> file(std::string("assets/sprites/" + sheetName + ".xml").c_str());
	doc.parse<0>(file.data());
	auto x = doc.first_node("TextureAtlas");
	// Check there is a texture atlas
	if (doc.first_node("TextureAtlas") != nullptr) {
		// Load it
		this->readAsSpriteSheet(sheetName, doc);
	}
	// The file may be a tileset
	// Check if it is
	if (doc.first_node("TileSetAtlas")) {
		this->readAsTileSheet(sheetName, doc);
	}
}
void ResourceManager::readAsSpriteSheet(std::string sheetName, rapidxml::xml_document<>& doc)
{
	rapidxml::xml_node<>* atlas = doc.first_node("TextureAtlas");
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
void ResourceManager::readAsTileSheet(std::string sheetName, rapidxml::xml_document<>& doc)
{
	// Get atlas node
	rapidxml::xml_node<>* atlas = doc.first_node("TileSetAtlas");
	// Get dimensions
	int s = std::stoi(atlas->first_attribute("size")->value());
	// Load texture
	this->sheets.insert({
		sheetName,
		{
			sf::Texture(),
			{}
		}
	});
	sf::Texture* t = &this->sheets[sheetName].first;
	t->loadFromFile("assets/sprites/" + std::string(atlas->first_attribute("path")->value()));
	// Cycle through nodes
	rapidxml::xml_node<>* tile = atlas->first_node("Tile");
	while (tile) {
		// Get tile attributes
		std::string name = tile->first_attribute("name")->value();
		int x = std::stoi(tile->first_attribute("x")->value());
		int y = std::stoi(tile->first_attribute("y")->value());
		// Create IntRect
		sf::IntRect r({ x * s, y * s}, { s, s });
		// Add IntRect
		this->sheets[sheetName].second.insert({
			name,
			r
		});
		// Go to next tile
		tile = tile->next_sibling("Tile");
	}
}
ResourceManager* ResourceManager::get()
{
	static ResourceManager r;
	return &r;
}