#include "MainMenu.h"

MainMenu::MainMenu(App* a) : app(a) {
	this->container = tgui::VerticalLayout::create();
	tgui::Button::Ptr loadBtn = tgui::Button::create();
	loadBtn->setText("Load test.xml");
	loadBtn->connect("clicked", &App::loadGame, this->app, "test.xml");
	this->container->add(loadBtn);
	tgui::Button::Ptr newBtn = tgui::Button::create();
	newBtn->setText("New Game");
	newBtn->connect("clicked", &App::newGame, this->app);
	this->container->add(newBtn);
}

void MainMenu::show() {
	this->app->getGui()->add(this->container);
}
void MainMenu::hide() {
	this->app->getGui()->remove(this->container);
}