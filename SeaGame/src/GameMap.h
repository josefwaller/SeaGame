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
	TileType getTileAt(size_t x, size_t y);
	sf::Vector2<size_t> getMapSize();
private:
	// The tiles in the map
	std::vector<std::vector<TileType>> tiles;
	// The Box2d bodies associated with the islands
	std::vector<b2Body*> bodies;
	sf::Texture texture;
	void GameMap::drawTile(sf::RenderTexture* rt, size_t x, size_t y);
	sf::Sprite GameMap::getSprite(TileType top, TileType right, TileType bottom, TileType left);
	std::string GameMap::getTileString(TileType t);
};

#include "Game.h"