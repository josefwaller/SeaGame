#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class ResourceManager
{
public:
	ResourceManager();

	static ResourceManager* get();
	sf::Sprite getSprite(std::string sheetName, std::string spriteName);

private:
	// Textures loaded, with individual sprite positions saved as IntRects
	std::map<std::string, std::pair<sf::Texture, std::map<std::string, sf::IntRect>>> sheets;
	void loadSpritesheet(std::string sheetName);
};