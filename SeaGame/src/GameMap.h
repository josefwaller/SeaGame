#pragma once
#include <SFML/Graphics.hpp>
#include "RenderManager.h"

class GameMap
{
public:
	GameMap();
	void render(RenderManager& r);
private:
	sf::Sprite seaSpr;
};