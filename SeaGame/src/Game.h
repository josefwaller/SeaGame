#pragma once
#include "Box2D/Box2D.h"
#include <SFML\Graphics.hpp>
#include "GameMap.h"
#include "GameHud.h"
#include "Entity.h"
#include "SimpleCollisionListener.h"
#include "GameResource.h"
#include <TGUI/TGUI.hpp>

class Game
{
public:
	Game(sf::RenderWindow& window, tgui::Gui& gui);
	void update(double delta);
	void render();

	// Add an entity to the game
	void addEntity(std::shared_ptr<Entity> newEnt);
	// Remove an entity from the game
	void removeEntity(std::weak_ptr<Entity> e);
	// Handle an event
	void handleEvent(sf::Event e);
	// Get mouse coordinates relative to the window
	sf::Vector2f getMouseCoords();
	// Get player
	std::shared_ptr<Entity> getPlayer();
	// Get all entities
	std::vector<std::shared_ptr<Entity>> getEntities();
	// Get the box2d world
	std::weak_ptr<b2World> getWorld();
	// Get the GUI world for adding/removing widgets
	tgui::Gui& getGui();
	GameHud* getHud();
private:
	// Window for ref
	sf::RenderWindow& window;
	// TGUI handler
	tgui::Gui& gui;
	// The box2d world
	std::shared_ptr<b2World> world;
	// Box2d listener
	SimpleCollisionListener listener;
	// Pointer to the player's ship
	// Actual owner pointer is in entities
	std::weak_ptr<Entity> player;
	GameMap gMap;
	GameHud gHud;
	std::vector<std::shared_ptr<Entity>> entities;
	// Entities to be removed after this frame
	std::vector<std::weak_ptr<Entity>> toRemove;
	// The build button
	tgui::Button::Ptr buildBtn;
	// The buttons specifying what to build
	// Todo: think of a better name
	std::vector<tgui::Button::Ptr> buildThingsBtns;
};