#pragma once
#include <memory>
#include "SFML\Graphics.hpp"
#include "TGUI\TGUI.hpp"
#include "Game.h"
#include "MainMenu.h"

// App is the container for everything
// Only one ever exists, contains and creates a new game
// For each game created/loaded
class App {
public:
	enum AppState {
		InMenu,
		InGame
	};
	App();
	void gameLoop();
	void newGame();
	void loadGame(std::string fileName);
	sf::RenderWindow* getWindow();
	tgui::Gui* getGui();
private:
	sf::RenderWindow window;
	tgui::Gui gui;
	std::unique_ptr<Game> game;
	MainMenu menu;
	AppState state;
};