#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "ResourceManager.h"
#include "App.h"
int main()
{
	sf::RenderWindow w(sf::VideoMode(1920, 1080), "Sea Game");
	tgui::Gui g(w);
	App a(&w, &g);
	a.gameLoop();
	return 0;
}