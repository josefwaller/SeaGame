#pragma once
#include "TGUI\TGUI.hpp"
class App;

class MainMenu {
public:
	enum State {
		SplashScreen
	};
	MainMenu(App* app);
	void show();
	void hide();
	void render(sf::RenderWindow* window);
private:
	App* app;
	// All the stuff on the main menu
	tgui::Container::Ptr container;
	// Show the savefiles available in the save window
	void showSaveFiles();
};

#include "App.h"