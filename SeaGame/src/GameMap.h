#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "RenderManager.h"
// Forward declaration of game
class Game;

class GameMap
{
public:
	// Different types of tiles
	enum TileType
	{
		Sea,
		Land
	};

	GameMap(Game* g);
	GameMap();
	void render(RenderManager& r);
private:
	// The tiles in the map
	std::vector<std::vector<TileType>> tiles;
	// The Box2d bodies associated with the islands
	std::vector<b2Body*> bodies;
	// Get the string representation of the edge between two tiles
	// Used for reading the correct sprite
	std::string getEdgeType(sf::Vector2<size_t> one, sf::Vector2<size_t> two);
	// Check if an index has a sea tile in any of its 8 surronding tiles
	bool hasSeaAround(sf::Vector2<size_t> indexes);
};

#include "Game.h"