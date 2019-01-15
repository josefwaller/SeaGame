#pragma once
#include <rapidxml\rapidxml.hpp>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "RenderManager.h"
#include "SaveData.h"
#include <unordered_set>
#include <unordered_map>
#include "EntityType.h"

// Forward declaration of game
class Game;
struct Entity;

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
	GameMap(Game* g, SaveData data);
	GameMap();
	void render(sf::RenderWindow* window);
	TileType getTileAt(size_t x, size_t y);
	sf::Vector2<size_t> getMapSize();
	SaveData getSaveData();
	// Values that are rendered
	struct TileRenderData {
		sf::Sprite topLeft;
		sf::Sprite topRight;
		sf::Sprite botLeft;
		sf::Sprite botRight;
	};
	struct Tile {
		// The type (Land or sea) of this tile
		TileType type;
		// The data used to render it quickly
		TileRenderData data;
		// Whether or not the tile has something (i.e. a base or something) on it
		bool isFull;
		Tile() : type(TileType::Sea), data(), isFull(false) {}
		Tile(TileType t) : type(t), isFull(false) {}
	};
private:
	// Game the map is in
	Game* game;
	// The tiles in the map
	std::vector<std::vector<Tile>> tiles;
	// The Box2d bodies associated with the islands
	std::vector<b2Body*> bodies;
	// Set the TileRenderData for each tile, and add sprites to tileSprites
	void initTileRenderData();
	void GameMap::setTileRenderData(size_t x, size_t y);
	void GameMap::drawTile(sf::RenderWindow* window, size_t x, size_t y);
	sf::Sprite GameMap::getSprite(TileType top, TileType right, TileType bottom, TileType left);
	std::string GameMap::getTileString(TileType t);
	// Add a land tile at the position and create a box2d collider for it
	void addLandTile(size_t x, size_t y);
	// Add a given type of building somewhere in the bounds given
	// Return whether the building was successfully built
	bool addBuilding(
		EntityType type,
		size_t startX,
		size_t startY,
		size_t endX,
		size_t endY);
};

#include "Game.h"
#include "Entity.h"