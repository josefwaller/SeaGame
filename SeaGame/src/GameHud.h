#pragma once
#include "TGUI/TGUI.hpp"
// Forward dec of game + entity
class Game;
struct Entity;

// Manage the HUD interface, as well as selecting entities, etc
class GameHud {
public:
	GameHud();
	GameHud(Game* game);
	// The clicking states, help determine what to do when clicking
	enum ClickState {
		// Nothing, i.e. clicking on stuff will show their gui
		Nothing,
		// Choosing where to place a building
		Building,
		// Selecting a building as a source/destination
		Selecting
	};
	void onClick(sf::Vector2f pos);
	// Wait for a click, then call the callback with the point clicked
	void selectPoint(ClickState c, std::function<void(Game* g, sf::Vector2f pos)> func);
	// Select an entity, then call the callback method with the entity
	void selectEntity(std::function<void(std::weak_ptr<Entity>)> callback);
	// Show/hide the different buttons of things to build
	void toggleBuildButtons();
private:
	// The game the HUD belongs to
	Game* game;
	tgui::Button::Ptr buildButton;
	// Buttons to decide what to build
	std::vector<tgui::Button::Ptr> toBuildButtons;
	// Current Click state
	ClickState currentClickState;
	/*
	 *There are two ways to select something via clicking on it
	 * Selecting a point, or selecting an entity
	 */
	// Callback function when clicking on a point
	std::function<void(Game* g, sf::Vector2f pos)> clickCallback;
	// The callback for selecting an entity (by clicking on it)
	std::function<void(std::weak_ptr<Entity> entity)> selectCallback;
};
#include "Game.h"
#include "Entity.h"