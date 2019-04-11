#include "Tile.h"
#include "ResourceManager.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

void Tile::render(float x, float y, sf::RenderWindow* window) {
	TileRenderData data = this->data;
	const std::vector<sf::Sprite*> sprs = {
		&data.topLeft,
		&data.topRight,
		&data.botLeft,
		&data.botRight
	};
	sf::Vector2f pos = sf::Vector2f(x, y) * 64.0f;
	const std::vector<sf::Vector2f> poses = {
		pos,
		pos + sf::Vector2f(32.0f, 0),
		pos + sf::Vector2f(0, 32.0f),
		pos + sf::Vector2f(32.0f, 32.0f)
	};
	for (size_t i = 0; i < 4; i++) {
		sf::Sprite* s = sprs[i];
		s->setScale(0.5, 0.5);
		s->setPosition(poses[i]);
		window->draw(*s);
	}
}
sf::Sprite Tile::getTileSprite(TileType top, TileType right, TileType bottom, TileType left) {
	return ResourceManager::get()->getSprite("tiles",
		getTileString(top) + "-" +
		getTileString(right) + "-" +
		getTileString(bottom) + "-" +
		getTileString(left),
	false);
}
std::string Tile::getTileString(TileType t) {
	if (t == TileType::Land) {
		return "land";
	}
	else {
		return "sea";
	}
}
TileRenderData Tile::getTileRenderData(
	TileType top,
	TileType left,
	TileType bottom,
	TileType right,
	TileType center) {
	return {
		Tile::getTileSprite(top, center, center, left),
		Tile::getTileSprite(top, right, center, center),
		Tile::getTileSprite(center, center, bottom, left),
		Tile::getTileSprite(center, right, bottom, center)
	};
}
