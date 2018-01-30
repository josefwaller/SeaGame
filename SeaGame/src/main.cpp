#include <SFML/Graphics.hpp>
#include "ResourceManager.h"
#include "Game.h"
int main()
{
	sf::RenderWindow w(sf::VideoMode(1920, 1080), "Sea Game");
	Game g(w);
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
		g.update(dClock.restart().asSeconds());
		w.clear();
		g.render();
		w.display();
	}
	return 0;
}