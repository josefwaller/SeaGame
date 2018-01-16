#pragma once
#include <string>
#include <SFML/Graphics.hpp>

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
	void ensureSheetIsLoaded(std::string sheetName);
	void loadSpritesheet(std::string sheetName);
};