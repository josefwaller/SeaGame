#pragma once
#include <rapidxml\rapidxml.hpp>
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

	// Generate a new map
	GameMap(Game* g);
	// Get map data from a document
	GameMap(Game* g, rapidxml::xml_document<>* data);
	GameMap();
	void render(RenderManager& r);
	TileType getTileAt(size_t x, size_t y);
	sf::Vector2<size_t> getMapSize();
	void addSaveData(rapidxml::xml_document<>* doc);
private:
	// Game the map is in
	Game* game;
	// The tiles in the map
	std::vector<std::vector<TileType>> tiles;
	// The Box2d bodies associated with the islands
	std::vector<b2Body*> bodies;
	sf::Texture texture;
	void GameMap::drawTile(sf::RenderTexture* rt, size_t x, size_t y);
	sf::Sprite GameMap::getSprite(TileType top, TileType right, TileType bottom, TileType left);
	std::string GameMap::getTileString(TileType t);
	// Add a land tile at the position and create a box2d collider for it
	void addLandTile(size_t x, size_t y);
	void resetTexture();
};

#include "Game.h"