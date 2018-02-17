#include "GameMap.h"
#include "ResourceManager.h"

GameMap::GameMap()
{
	// Add tiles
	for (auto x = 0; x < 10; x++) {
		this->tiles.push_back({});
		for (auto y = 0; y < 10; y++) {
			// Add test island in the middle
			if (x < 7 && x > 3 && y < 9 && y > 3)
				this->tiles[x].push_back(TileType::Land);
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
			// First draw sea under it
			sf::Sprite seaSprite = ResourceManager::get()->getSprite("tiles", "sea-sea-sea-sea", false);
			seaSprite.setPosition({ 64 * (float)x, 64 * (float)y });
			r.add(seaSprite, RenderManager::INDEX_SEA_TILES);
			// Draw the land if there is any
			if (this->tiles[x][y] == TileType::Land) {
				// Create a vector of the string name of the tile needed
				std::vector<std::string> tileStringParts;
				// Get the types of the tiles around it
				std::vector<int> indexes = { -1, 0, 1, 0 };
				for (size_t i = 0; i < indexes.size(); i++) {
					size_t yOff = indexes[(size_t)i];
					size_t xOff = indexes[(size_t)(i + 1) % indexes.size()];
					// Check tile is on map
					if (x + xOff >= 0 && x + xOff < this->tiles.size()) {
						if (y + yOff >= 0 && y + yOff < this->tiles[x].size()) {
							// Add string representation of edge between these two tiles
							// e.g. "sea", "land", "shore"
							tileStringParts.push_back(this->getEdgeType({ x, y }, { x + xOff, y + yOff }));
							continue;
						}
					}
					// Assume edge is sea if it is not on the map
					tileStringParts.push_back("sea");
				}
				// Add dashes between string
				std::string tileString = tileStringParts[0];
				for (size_t i = 1; i < tileStringParts.size(); i++) {
					tileString += "-" + tileStringParts[i];
				}
				// Get the tile
				sf::Sprite s = ResourceManager::get()->getSprite("tiles", tileString, false);
				s.setPosition(
					x * (float)s.getTextureRect().width,
					y * (float)s.getTextureRect().height
				);
				// Add it to be drawn
				r.add(s, RenderManager::INDEX_LAND_TILES);
			}
		}
	}
}
std::string GameMap::getEdgeType(sf::Vector2<size_t> one, sf::Vector2<size_t> two)
{
	TileType tOne = this->tiles[one.x][one.y];
	TileType tTwo = this->tiles[two.x][two.y];
	if (tOne == TileType::Sea || tTwo == TileType::Sea) {
		return "sea";
	} 
	else {
		if (this->hasSeaAround(one) && this->hasSeaAround(two))
			return "shore";
		return "land";
	}
}
bool GameMap::hasSeaAround(sf::Vector2<size_t> indexes) {
	std::vector<int> offsets = { -1, 0, 1 };

	for (int x : offsets) {
		for (int y : offsets) {
			if (x != 0 || y != 0) {
				if (this->tiles[indexes.x + x][indexes.y + y] == TileType::Sea)
					return true;
			}
		}
	}
	return false;
}