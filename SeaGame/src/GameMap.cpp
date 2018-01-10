#include "GameMap.h"
#include "ResourceManager.h"

GameMap::GameMap()
{
	// Get the sea sprite
	this->seaSpr = ResourceManager::get()->getSprite("tiles", "sea");
}
void GameMap::render(sf::RenderWindow& w)
{
	// Render a 10x10 grid of sea tiles
	for (size_t x = 0; x < 10; x++)
	{
		for (size_t y = 0; y < 10; y++)
		{
			this->seaSpr.setPosition(
				x * this->seaSpr.getTextureRect().width,
				y * this->seaSpr.getTextureRect().height
			);
			w.draw(this->seaSpr);
		}

	}
}