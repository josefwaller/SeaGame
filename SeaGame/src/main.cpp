#include <SFML/Graphics.hpp>
#include "ResourceManager.h"
#include "Game.h"

int main()
{
	sf::RenderWindow w(sf::VideoMode(800, 600), "Sea Game");
	Game g;
	// Delta time
	sf::Clock dClock;
	while (w.isOpen())
	{
		sf::Event e;
		while (w.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				w.close();
		}
		g.update(dClock.getElapsedTime().asMilliseconds());
		w.clear();
		g.render(w);
		w.display();
	}
	return 0;
}