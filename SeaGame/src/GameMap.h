#pragma once
#include <SFML/Graphics.hpp>

class GameMap
{
public:
	GameMap();
	void render(sf::RenderWindow& w);
private:
	sf::Sprite seaSpr;
};