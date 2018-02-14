#include "GameMap.h"
#include "ResourceManager.h"

// Names of the various different types of tiles
std::map<GameMap::TileType, sf::Sprite> GameMap::tileSprites = []() {
	// Get the names of the sprites to load
	std::map<TileType, std::string> spriteNames =
	{
		{
			TileType::Sea,
			"sea"
		},
		{
			TileType::Sand,
			"sand"
		},
		{
			TileType::Grass,
			"grass"
		}
	};
	std::map<TileType, sf::Sprite> toReturn;
	// Set sprite values
	for (auto p : spriteNames) {
		// Create new sprite
		sf::Sprite s;
		// Set attributes
		auto spriteInfo = ResourceManager::get()->getSpriteInfo("tiles", p.second);
		s.setTexture(*spriteInfo.first);
		s.setTextureRect(spriteInfo.second);
		// Add to tileSprites
		toReturn.insert({ p.first, s });
	}
	return toReturn;
}();
GameMap::GameMap()
{
	// Add tiles
	for (auto x = 0; x < 10; x++) {
		this->tiles.push_back({});
		for (auto y = 0; y < 10; y++) {
			// Add test island in the middle
			if (x < 7 && x > 3 && y < 7 && y > 3)
				if (x == 5 && y == 5)
					this->tiles[x].push_back(TileType::Grass);
				else
					this->tiles[x].push_back(TileType::Sand);
			else
				this->tiles[x].push_back(TileType::Sea);
		}
	}
}
void GameMap::render(RenderManager& r)
{
	// Render a 10x10 grid of sea tiles
	for (size_t x = 0; x < this->tiles.size(); x++)
	{
		for (size_t y = 0; y < this->tiles[x].size(); y++)
		{
			sf::Sprite s = tileSprites[this->tiles[x][y]];
			s.setPosition(
				x * (float)s.getTextureRect().width,
				y * (float)s.getTextureRect().height
			);
			r.add(s, RenderManager::INDEX_TILES);
		}
	}
}