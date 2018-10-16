#pragma once
#include "TGUI\TGUI.hpp"
class App;

class MainMenu {
public:
	enum State {
		SplashScreen
	};
	MainMenu(App* g);
	void loadGame(std::string fileName);
	void newGame();
private:
	// All the stuff on the main menu
	tgui::VerticalLayout::Ptr container;
};

#include "App.h"