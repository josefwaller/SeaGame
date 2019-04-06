#pragma once
#include "TGUI/TGUI.hpp"
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
	// Render method, draws the lines between buttons
	void render(RenderManager& w);
private:
	// The research buttons
	tgui::Group::Ptr researchBtns;
	// The game
	Game* game;
	// The lines between technolgies showing parent/child relationships
	sf::VertexArray techLines;
	// Get the button for a certain tech
	tgui::Button::Ptr getButtonForTech(Technology t);
};