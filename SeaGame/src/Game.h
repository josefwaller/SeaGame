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
	// Get player
	std::shared_ptr<Entity> getPlayer();
	// Get all entities
	std::vector<std::shared_ptr<Entity>> getEntities();
private:
	// Window for ref
	sf::RenderWindow& window;
	// Pointer to the player's ship
	// Actual owner pointer is in entities
	std::weak_ptr<Entity> player;
	GameMap gMap;
	std::vector<std::shared_ptr<Entity>> entities;
};