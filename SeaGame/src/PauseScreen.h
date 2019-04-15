#pragma once
#include <TGUI/Container.hpp>
// Forward Declarations
class Game;

class PauseScreen {
public:
	PauseScreen();
	PauseScreen(Game* g);
	void show(tgui::Container::Ptr c);
	void hide(tgui::Container::Ptr c);
private:
	tgui::Widget::Ptr screen;
};
