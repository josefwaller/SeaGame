#pragma once
#include "ResearchScreen.h"
#include "BuildScreen.h"
#include "PauseScreen.h"
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
	// The width of the announcement widget
	static const float ANNOUNCEMENT_WIDTH;
	// The height of each announcement
	static const float ANNOUNCEMENT_ITEM_HEIGHT;
	GameHud();
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
	// Transfer items between entities by clicking between them
	void transferItems(std::weak_ptr<Entity> e, GameResource res, unsigned int amount);
	// Add an announcement
	void addAnnouncement(std::string text);
	// Get the entity GUI panel
	tgui::ChildWindow::Ptr getEntityPanel();
private:
	// The game the HUD belongs to
	Game* game;
	// Build button
	tgui::Button::Ptr buildButton;
	// Button to toggle research screen
	tgui::Button::Ptr researchButton;
	// Money display
	tgui::Label::Ptr moneyDisplay;
	// The state text ("Choose where to build the entity", etc)
	tgui::Label::Ptr stateText;
	// Player health display
	tgui::ProgressBar::Ptr playerHealth;
	// Current Click state
	State currentState;
	tgui::VerticalLayout::Ptr announcementContainer;
	// The entity gui panel
	tgui::ChildWindow::Ptr entityPanel;
	// The research screen
	ResearchScreen researchScreen;
	// The build screen
	BuildScreen buildScreen;
	// The pause screen
	PauseScreen pauseScreen;
	/*
	 * There are two ways to select something via clicking on it
	 * Selecting a point, or selecting an entity
	 */
	// Callback function when clicking on a point
	std::function<void(Game* g, sf::Vector2f pos)> clickCallback;
	// The callback for selecting an entity (by clicking on it)
	std::function<void(std::weak_ptr<Entity> entity)> selectCallback;
	// The announcements being shown
	// Hold the GUI component and the clock in order to check when to destory it
	std::queue<std::pair<tgui::TextBox::Ptr, sf::Clock>> announcements;
};