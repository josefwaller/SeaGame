#pragma once
#include "EntityType.h"
#include "Tile.h"
#include <vector>
#include <string>
// Cannot forward declare Vector2 or Sprite, so just include it
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>

// Forward declarations
class Game;
class SaveData;
struct Entity;
class b2Body;
namespace sf {
	class RenderWindow;
};

class GameMap
{
public:
	// Generate a new map
	GameMap(Game* g);
	// Get map data from a document
	GameMap(Game* g, SaveData data);
	// Set the entity for each tile when loading
	// Has to be done after intializing the game, so that the entities exist
	void initTileEntities(SaveData data);
	GameMap();
	void render(sf::RenderWindow* window);
	// Get the tile type at a coordinate
	TileType getTileAt(size_t x, size_t y);
	// Get whether the coordinate has an entity on it
	// Only for static entities
	bool getTileIsFull(size_t x, size_t y);
	// Set whether the tile is full at a certain coord
	void setTileEntity(size_t x, size_t y, std::weak_ptr<Entity>);
	// Get the entity for a tile from its coords
	std::weak_ptr<Entity> getTileEntity(size_t x, size_t y);
	// Get the size of the map
	sf::Vector2<size_t> getMapSize();
	SaveData getSaveData();
private:
	// Game the map is in
	Game* game;
	// The tiles in the map
	std::vector<std::vector<Tile>> tiles;
	// Check whether an x, y coordinate is on the map
	bool coordIsOnMap(size_t x, size_t y);
	// The Box2d bodies associated with the islands
	std::vector<b2Body*> bodies;
	// Set the TileRenderData for each tile, and add sprites to tileSprites
	void initTileRenderData();
	void setTileRenderData(size_t x, size_t y);
	void drawTile(sf::RenderWindow* window, size_t x, size_t y);
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