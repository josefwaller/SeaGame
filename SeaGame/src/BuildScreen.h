#pragma once
#include <TGUI/Container.hpp>
#include "CraftRecipes.h"
// Forward declarations
class Game;
class RenderManager;
struct Entity;

class BuildScreen {
public:
	BuildScreen();
	BuildScreen(Game* g);
	// Update the information on the screen
	void update();
	// Render things that are not gui
	void render(RenderManager& rm);
	// Show/Hide the screen
	void show(tgui::Container::Ptr c);
	void hide(tgui::Container::Ptr c);
	// Show the entity the player is trying to build
	void showCurrentBuild(RenderManager& rm);
	// Handle a click
	void onClick(sf::Vector2f pos);
	// Set the entity to build
	void setToBuild(CraftingRecipes::CraftRecipe cr);
private:
	// The game
	Game* game;
	// The buttons to determine what to build
	tgui::Container::Ptr buttons;
	// Whether the player is building something right now
	bool isBuilding;
	// What the player is building right now
	CraftingRecipes::CraftRecipe buildingRecipe;
	// A modified Entity with no components except transform and renderer,
	// used to display where the entity will be
	std::shared_ptr<Entity> toBuild;
	// Whether an entity's position is valid
	bool ensureValid(std::shared_ptr<Entity> e);
};
