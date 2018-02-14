#pragma once
#include <SFML/Graphics.hpp>
#include "RenderManager.h"

class GameMap
{
public:
	// Different types of tiles
	enum TileType
	{
		Sea,
		Sand,
		Grass
	};
	// The different sprites used for each tile
	static std::map<GameMap::TileType, sf::Sprite> tileSprites;

	GameMap();
	void render(RenderManager& r);
private:
	// The tiles in the map
	std::vector<std::vector<TileType>> tiles;
};