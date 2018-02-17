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
		Land
	};

	GameMap();
	void render(RenderManager& r);
private:
	// The tiles in the map
	std::vector<std::vector<TileType>> tiles;
	// Get the string representation of the edge between two tiles
	// Used for reading the correct sprite
	std::string getEdgeType(sf::Vector2<size_t> one, sf::Vector2<size_t> two);
	// Check if an index has a sea tile in any of its 8 surronding tiles
	bool hasSeaAround(sf::Vector2<size_t> indexes);
};