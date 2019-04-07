#pragma once
#include <TGUI/Widgets/ScrollablePanel.hpp>
#include <TGUI/Container.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include "TechTree.h"
class Game;
class RenderManager;
/*
 * Shows and hides the research buttons
 */
class ResearchScreen {
public:
	ResearchScreen();
	ResearchScreen(Game* g);
	// Show the research screen
	void show(tgui::Container::Ptr c);
	// Hide the research screen
	void hide(tgui::Container::Ptr c);
	// Update whether the buttons are enabled or disabled
	void update();
	// Render method, draws the lines between buttons
	void render(RenderManager& w);
private:
	// The research buttons
	tgui::ScrollablePanel::Ptr researchBtns;
	// The button to close the screen
	tgui::Button::Ptr closeBtn;
	// Pointer to each button by technology
	std::map<Technology, tgui::Button::Ptr> techButtons;
	// The game
	Game* game;
	// The lines between technolgies showing parent/child relationships
	sf::VertexArray techLines;
	// Get the button for a certain tech
	tgui::Button::Ptr getButtonForTech(Technology t);
};