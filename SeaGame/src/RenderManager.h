#pragma once
#include <SFML/Graphics.hpp>

class RenderManager
{
public:
	// Draw all sprites on the window
	void render(sf::RenderWindow& window);
	// Add a sprite to be drawn
	void addSprite(sf::Sprite s, size_t zIndex);
	// Remove all sprites
	void reset();
	
	// Z indexes to draw things on
	// For sea tiles, land tiles, etc
	const static size_t INDEX_TILES = 0;
	// For ship hulls
	const static size_t INDEX_HULL = 1;
	// For cannonballs
	const static size_t INDEX_CANNONBALLS = 2;
	// For things on the decks of ships
	const static size_t INDEX_DECK = 3;
	// For the ship sails
	const static size_t INDEX_SAILS = 4;
private:
	// Sprites to draw, ordered by z index
	std::vector<std::vector<sf::Sprite>> sprites;
};