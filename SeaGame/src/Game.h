#pragma once
#include "Box2D/Box2D.h"
#include <SFML\Graphics.hpp>
#include "GameMap.h"
#include "Entity.h"
#include "SimpleCollisionListener.h"

class Game
{
public:
	Game(sf::RenderWindow& window);
	void update(double delta);
	void render();

	// Add an entity to the game
	void addEntity(std::shared_ptr<Entity> newEnt);
	// Remove an entity from the game
	void removeEntity(std::weak_ptr<Entity> e);
	// Get mouse coordinates relative to the window
	sf::Vector2f getMouseCoords();
	// Get player
	std::shared_ptr<Entity> getPlayer();
	// Get all entities
	std::vector<std::shared_ptr<Entity>> getEntities();
	// Get the box2d world
	std::weak_ptr<b2World> getWorld();
private:
	// Window for ref
	sf::RenderWindow& window;
	// The box2d world
	std::shared_ptr<b2World> world;
	// Box2d listener
	SimpleCollisionListener listener;
	// Pointer to the player's ship
	// Actual owner pointer is in entities
	std::weak_ptr<Entity> player;
	GameMap gMap;
	std::vector<std::shared_ptr<Entity>> entities;
	// Entities to be removed after this frame
	std::vector<std::weak_ptr<Entity>> toRemove;
};