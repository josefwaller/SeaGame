#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "LayoutSprite.h"
#include <rapidxml\rapidxml.hpp>

class ResourceManager
{
public:
	ResourceManager();

	static ResourceManager* get();
	std::pair<const sf::Texture*, sf::IntRect> getSpriteInfo(std::string sheetName, std::string spriteName);
	std::map<std::string, LayoutSprite> getLayout(std::string layoutName);
	sf::Sprite getSprite(std::string sheetName, std::string spriteName, bool center);
private:
	// Textures loaded, with individual sprite positions saved as IntRects
	std::map<std::string, std::pair<sf::Texture, std::map<std::string, sf::IntRect>>> sheets;
	// Attributes for each sprite
	// For example, x and y coordinates for each different hull that the sail should be drawn on
	// Keys are certain keywords, i.e. hull, which are used by the renderer to determine which sprites to draw
	std::map<std::string, std::map<std::string, LayoutSprite>> layouts;
	void ensureSheetIsLoaded(std::string sheetName);
	void loadSpritesheet(std::string sheetName);
	// Read file as a spritesheet and add it to sprites
	void readAsSpriteSheet(std::string sheetName, rapidxml::xml_document<>& doc);
	// Read file as a tilesheet and add it to sprites
	void readAsTileSheet(std::string sheetName, rapidxml::xml_document<>& doc);
};