#pragma once
#include "TGUI/TGUI.hpp"
// Forward dec of game + entity
class Game;
struct Entity;
#include "CraftRecipes.h"
#include "RenderManager.h"

// Manage the HUD interface, as well as selecting entities, etc
class GameHud {
public:
	// The width of the announcement widget
	static const float ANNOUNCEMENT_WIDTH;
	// The height of each announcement
	static const float ANNOUNCEMENT_ITEM_HEIGHT;
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
	// Update widgets that need updating
	void update();
	// Render things not in the GUI, i.e. the entity being built
	void render(RenderManager& rm);
	// Chose an entity to build on the next click
	void chooseEntityToBuild(CraftingRecipes::CraftRecipe cr);
	// Build an entity from a crafting recipe given
	void buildEntity();
	void onClick(sf::Vector2f pos);
	// Select an entity, then call the callback method with the entity
	void selectEntity(std::function<void(std::weak_ptr<Entity>)> callback);
	// Show/hide the different buttons of things to build
	void toggleBuildButtons();
	void toggleResearchButtons();
	// Reset the build buttons
	void resetBuildButtons();
	// Reset the research buttons
	void resetResearchButtons();
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
	// The buttons to decide what to build
	tgui::Group::Ptr buildGroup;
	// Button to show what to research
	tgui::Button::Ptr researchButton;
	// The buttons to decide what to research
	tgui::Group::Ptr researchGroup;
	// Save Button
	tgui::Button::Ptr saveBtn;
	// Money display
	tgui::Label::Ptr moneyDisplay;
	// The state text ("Choose where to build the entity", etc)
	tgui::Label::Ptr stateText;
	// Player health display
	tgui::ProgressBar::Ptr playerHealth;
	// Check if an entity is valid and can be built
	bool ensureValid(std::shared_ptr<Entity> e);
	// Current Click state
	ClickState currentClickState;
	// A modified version of the entity with only a render and transform component,
	// used to show the entity hovering at the moues coords
	std::shared_ptr<Entity> toBuild;
	// The crafting recipe of the entity currently being build
	CraftingRecipes::CraftRecipe buildRecipe;
	tgui::VerticalLayout::Ptr announcementContainer;
	// The entity gui panel
	tgui::ChildWindow::Ptr entityPanel;
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
#include "Game.h"
#include "Entity.h"