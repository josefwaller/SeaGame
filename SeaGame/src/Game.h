#pragma once
#include <SFML\Graphics.hpp>
#include "GameMap.h"

class Game
{
public:
	Game();
	void update(unsigned int delta);
	void render(sf::RenderWindow& w);

private:
	GameMap gMap;
};