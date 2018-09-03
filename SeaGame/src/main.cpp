#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "ResourceManager.h"
#include "Game.h"
int main()
{
	sf::RenderWindow w(sf::VideoMode(1920, 1080), "Sea Game");
	tgui::Gui gui(w);
	Game g(w, gui);
	// Delta time
	sf::Clock dClock;
	while (w.isOpen())
	{
		sf::Event e;
		while (w.pollEvent(e))
		{
			gui.handleEvent(e);
			if (e.type == sf::Event::Closed)
				w.close();
			else
				g.handleEvent(e);
		}
		g.update(dClock.restart().asSeconds());
		w.clear();
		g.render();
		w.display();
	}
	return 0;
}