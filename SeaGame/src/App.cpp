#include "App.h"

App::App() :
	menu(this),
	window(sf::VideoMode(1920, 1080), "SeaGame"),
	gui(this->window) {

	this->state = AppState::InMenu;
	this->menu.show();
}
void App::gameLoop() {
	// Delta time
	sf::Clock dClock;
	while (this->window.isOpen())
	{
		sf::Event e;
		while (this->window.pollEvent(e))
		{
			bool clickedGui = this->gui.handleEvent(e);
			if (e.type == sf::Event::Closed)
				this->window.close();
			else if (!clickedGui && this->state == AppState::InGame)
				this->game->handleEvent(e);
		}
		if (this->state == AppState::InGame) {
			this->game->update(dClock.restart().asSeconds());
		}
		this->window.clear();
		if (this->state == AppState::InGame) {
			this->game->render();
		}
		// Render GUI
		this->gui.draw();
		this->window.display();
	}

}
void App::loadGame(std::string fileName) {
	this->menu.hide();
	this->game = std::unique_ptr<Game>(new Game(this));
	this->game->loadFromFile(fileName);
	this->state = AppState::InGame;
}
void App::newGame() {
	this->state = AppState::InGame;
	this->menu.hide();
	this->game = std::unique_ptr<Game>(new Game(this));
	this->game->generateNew();
}
sf::RenderWindow* App::getWindow() {
	return &this->window;
}
tgui::Gui* App::getGui() {
	return &this->gui;
}