#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <memory>
#include <string>
// Forward declarations
struct Entity;
namespace sf {
	class RenderWindow;
};

// Different types of tiles
enum TileType
{
	Sea,
	Land
};
// Values that are rendered
struct TileRenderData {
	sf::Sprite topLeft;
	sf::Sprite topRight;
	sf::Sprite botLeft;
	sf::Sprite botRight;
};
// Base struct that the map uses to hold data
struct Tile {
	// The type (Land or sea) of this tile
	TileType type;
	// The data used to render it quickly
	TileRenderData data;
	// The thing on the tile (only static things) or nullptr if there is nothing
	std::weak_ptr<Entity> entity;
	Tile() : type(TileType::Sea), data() {}
	Tile(TileType t) : type(t) {}
	// Render method
	void render(float x, float y, sf::RenderWindow* window);
	static sf::Sprite getTileSprite(TileType top, TileType left, TileType bot, TileType right);
	static std::string getTileString(TileType t);
	static TileRenderData getTileRenderData(TileType t, TileType l, TileType b, TileType r, TileType c);
};

