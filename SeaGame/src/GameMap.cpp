#include "GameMap.h"
#include "ResourceManager.h"

GameMap::GameMap()
{
	// Get the sea sprite
	this->seaSpr = sf::Sprite();
	auto spriteInfo = ResourceManager::get()->getSpriteInfo("tiles", "sea");
	this->seaSpr.setTexture(*spriteInfo.first);
	this->seaSpr.setTextureRect(spriteInfo.second);
}
void GameMap::render(RenderManager& r)
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
			r.add(this->seaSpr, RenderManager::INDEX_TILES);
		}

	}
}