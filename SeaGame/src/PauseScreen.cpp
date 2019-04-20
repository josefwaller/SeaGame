#include "PauseScreen.h"
#include "Game.h"
#include <TGUI/Widgets/ChildWindow.hpp>
#include <TGUI/Widgets/VerticalLayout.hpp>
#include <TGUI/SignalImpl.hpp>

PauseScreen::PauseScreen(Game* g) {
	auto gui = g->getGui();
	auto cW = tgui::ChildWindow::create();
	cW->setSize(400, 600);
	cW->setResizable(false);
	cW->setPositionLocked(true);
	cW->setPosition(
		(tgui::bindWidth(gui) - tgui::bindWidth(cW)) / 2,
		(tgui::bindHeight(gui) - tgui::bindHeight(cW)) / 2
	);
	// Add horizontal layout inside
	auto vL = tgui::VerticalLayout::create();
	cW->add(vL);
	// Add save button
	auto saveBtn = tgui::Button::create();
	saveBtn->setText("Save");
	saveBtn->connect("clicked", &Game::save, g);
	vL->add(saveBtn);
	// Add quit button
	auto quitBtn = tgui::Button::create();
	quitBtn->setText("Quit");
	quitBtn->connect("clicked", &Game::quitGame, g);
	vL->add(quitBtn);
	cW->connect("closed", &GameHud::hidePause, g->getHud());
	this->screen = cW;
}
void PauseScreen::show(tgui::Container::Ptr c) {
	c->add(this->screen);
}
void PauseScreen::hide(tgui::Container::Ptr c) {
	c->remove(this->screen);
}