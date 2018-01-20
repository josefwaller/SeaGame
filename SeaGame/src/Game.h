#pragma once
#include <SFML\Graphics.hpp>
#include "GameMap.h"
#include "Entity.h"

class Game
{
public:
	Game();
	void update(double delta);
	void render(sf::RenderWindow& w);

	// Add an entity to the game
private:
	GameMap gMap;
	std::vector<std::shared_ptr<Entity>> entities;
};