#include "App.h"

App::App(sf::RenderWindow* window, tgui::Gui* gui) :
	menu(this),
	window(sf::VideoMode(1920, 1080), "SeaGame"),
	gui(this->window),
	game(this) {
}
void App::gameLoop() {
	// Delta time
	sf::Clock dClock;
	while (this->window.isOpen())
	{
		sf::Event e;
		while (this->window.pollEvent(e))
		{
			this->gui.handleEvent(e);
			if (e.type == sf::Event::Closed)
				this->window.close();
			else
				this->game.handleEvent(e);
		}
		this->game.update(dClock.restart().asSeconds());
		this->window.clear();
		this->game.render();
		this->window.display();
	}

}
sf::RenderWindow* App::getWindow() {
	return &this->window;
}
tgui::Gui* App::getGui() {
	return &this->gui;
}