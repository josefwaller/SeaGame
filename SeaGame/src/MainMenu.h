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
private:
	App* app;
	// All the stuff on the main menu
	tgui::VerticalLayout::Ptr container;
	// Show the savefiles available in the save window
	void showSaveFiles();
};

#include "App.h"