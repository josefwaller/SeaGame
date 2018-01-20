#pragma once
#include <SFML\Graphics.hpp>
#include "GameMap.h"
#include "Entity.h"

class Game
{
public:
	Game(sf::RenderWindow& window);
	void update(double delta);
	void render();

	// Add an entity to the game
	void addEntity(std::shared_ptr<Entity> newEnt);
	// Get mouse coordinates relative to the window
	sf::Vector2f getMouseCoords();
private:
	// Window for ref`
	sf::RenderWindow& window;
	GameMap gMap;
	std::vector<std::shared_ptr<Entity>> entities;
};