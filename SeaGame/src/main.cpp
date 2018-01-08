#include <SFML/Graphics.hpp>
#include "ResourceManager.h"

int main()
{
	sf::RenderWindow w(sf::VideoMode(800, 600), "Sea Game");
	// Test load texture
	sf::Sprite spr = ResourceManager::get()->getSprite("tiles", "sea");
	spr.setPosition(10, 10);
	while (w.isOpen())
	{
		sf::Event e;
		while (w.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				w.close();
		}
		w.clear();
		w.draw(spr);
		w.display();
	}
	return 0;
}