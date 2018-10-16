#pragma once
#include "SFML\Graphics.hpp"
#include "TGUI\TGUI.hpp"
#include "Game.h"
#include "MainMenu.h"

// App is the container for everything
// Only one ever exists, contains and creates a new game
// For each game created/loaded
class App {
public:
	App(sf::RenderWindow* window, tgui::Gui* gui);
	void gameLoop();
	sf::RenderWindow* getWindow();
	tgui::Gui* getGui();
private:
	sf::RenderWindow window;
	tgui::Gui gui;
	Game game;
	MainMenu menu;
};