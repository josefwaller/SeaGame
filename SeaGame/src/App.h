#pragma once
#include <memory>
#include "SFML\Graphics.hpp"
#include <TGUI/Gui.hpp>
#include <TGUI/Loading/Theme.hpp>
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
	void quitGame();
	sf::RenderWindow* getWindow();
	tgui::Gui* getGui();
	// Get the main menu
	const MainMenu* getMainMenu();
private:
	sf::RenderWindow window;
	tgui::Gui gui;
	std::unique_ptr<Game> game;
	// The theme used for everything
	tgui::Theme theme;
	MainMenu menu;
	AppState state;
};