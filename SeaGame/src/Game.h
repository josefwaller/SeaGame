#pragma once
#include "Box2D/Box2D.h"
#include <SFML\Graphics.hpp>
#include "GameMap.h"
#include "GameHud.h"
#include "Entity.h"
#include "SimpleCollisionListener.h"
#include "GameResource.h"
#include <TGUI/TGUI.hpp>
#include "TechTree.h"
//For dec of app
class App;

class Game
{
public:
	static const unsigned int STARTING_MONEY = 20000;
	// Convert between world units and box2d units
	static const float BOX2D_TO_WORLD;
	static const float WORLD_TO_BOX2D;
	Game();
	~Game();
	Game(App* app);
	void loadFromData(
		GameMap gm,
		std::vector<std::shared_ptr<Entity>> entities,
		std::weak_ptr<Entity> player,
		TechTree techTree,
		unsigned int money
	);
	// Generate a new map, resources, etc
	void generateNew();
	// Set the gamemap to the one given, should only be called by save file
	void setGameMap(GameMap gm);
	void loadFromFile(std::string fileName);
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
	// Get entity by id
	std::weak_ptr<Entity> getEntityById(unsigned long id);
	// Get the box2d world
	std::weak_ptr<b2World> getWorld();
	// Get the GUI world for adding/removing widgets
	tgui::Group::Ptr getGui();
	GameHud* getHud();
	GameMap* getGameMap();
	TechTree* getTechTree();
	void save();
	// Get the amount of money the player has
	unsigned int getMoney();
	// Add/remove money
	void addMoney(unsigned int amount);
	void removeMoney(unsigned int amount);
	// Get window reference
	sf::RenderWindow* getWindow();
	// Get view reference
	sf::View* getView();
	// Get the rectangle fo the game the view shows
	// Used to only render things on screen
	sf::FloatRect getViewRect();
private:
	// App of the game, get window and gui through this pointer
	App* app;
	// Container of all gui items the game uses
	tgui::Group::Ptr guiContainer;
	// The box2d world
	std::shared_ptr<b2World> world;
	// Box2d listener
	SimpleCollisionListener listener;
	// Pointer to the player's ship
	// Actual owner pointer is in entities
	std::weak_ptr<Entity> player;
	GameMap gMap;
	GameHud gHud;
	TechTree techTree;
	std::vector<std::shared_ptr<Entity>> entities;
	// Entities to be removed after this frame
	std::vector<std::weak_ptr<Entity>> toRemove;
	// Theme for GUI
	tgui::Theme theme;
	// View for rendering game
	sf::View view;
	// The build button
	tgui::Button::Ptr buildBtn;
	// FPS text
	tgui::TextBox::Ptr fpsText;
	// The buttons specifying what to build
	// Todo: think of a better name
	std::vector<tgui::Button::Ptr> buildThingsBtns;
	// The amount of money the player has
	unsigned int money;
};