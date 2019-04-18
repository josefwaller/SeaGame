#pragma once
#include "ResearchScreen.h"
#include "BuildScreen.h"
#include "PauseScreen.h"
#include "DefaultScreen.h"
#include <TGUI/Widgets/ChildWindow.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/ProgressBar.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/VerticalLayout.hpp>
#include <TGUI/Widgets/TextBox.hpp>
#include <queue>
#include <functional>
#include <utility>
// Forward Declarations
class Game;
struct Entity;
class RenderManager;

// Manage the HUD interface, as well as selecting entities, etc
class GameHud {
public:
	GameHud(Game* game);
	// The state of what is being shown on the UI
	enum State {
		// Default, shows nothing
		Nothing,
		// The building screen
		Building,
		// Selecting a building as a source/destination
		Selecting,
		// The research screen
		Research,
		// Showing the pause screen (i.e. menu)
		Paused
	};
	// Update widgets that need updating
	void update();
	// Render things not in the GUI, i.e. the entity being built
	void render(RenderManager& rm);
	void onClick(sf::Vector2f pos);
	// Select an entity, then call the callback method with the entity
	void selectEntity(std::function<void(std::weak_ptr<Entity>)> callback);
	// Hide the screen that is currently being shown
	void hideCurrent();
	// Show/Hide the research screen
	void showResearch();
	void hideResearch();
	// Show/Hide the build buttons
	void showBuild();
	void hideBuild();
	void updateBuild();
	// Show/hide the menu screen
	void showPause();
	void hidePause();
	// Show/hide the default screen
	void showDefault();
	void hideDefault();
	// Transfer items between entities by clicking between them
	void transferItems(std::weak_ptr<Entity> e, GameResource res, unsigned int amount);
	// Add an announcement
	void addAnnouncement(std::string text);
	// Show an entity in the GUI
	void showEntity(std::weak_ptr<Entity> e);
private:
	// The game the HUD belongs to
	Game* game;
	// Current Click state
	State currentState;
	tgui::VerticalLayout::Ptr announcementContainer;
	// The research screen
	ResearchScreen researchScreen;
	// The build screen
	BuildScreen buildScreen;
	// The pause screen
	PauseScreen pauseScreen;
	// The default screen
	DefaultScreen defaultScreen;
	/*
	 * There are two ways to select something via clicking on it
	 * Selecting a point, or selecting an entity
	 */
	// Callback function when clicking on a point
	std::function<void(Game* g, sf::Vector2f pos)> clickCallback;
	// The callback for selecting an entity (by clicking on it)
	std::function<void(std::weak_ptr<Entity> entity)> selectCallback;
};