#pragma once
#include <TGUI/Container.hpp>
#include "CraftRecipes.h"
// Forward declarations
class Game;
class RenderManager;
struct Entity;

class BuildScreen {
public:
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
	// The sprite to show when building the entity
	sf::Sprite buildSprite;
};
