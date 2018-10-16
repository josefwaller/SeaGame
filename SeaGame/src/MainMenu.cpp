#include "MainMenu.h"

MainMenu::MainMenu(App* g) {
	this->container = tgui::VerticalLayout::create();
	tgui::Button::Ptr loadBtn = tgui::Button::create();
	loadBtn->setText("Load test.xml");
	this->container->add(loadBtn);
	tgui::Button::Ptr newBtn = tgui::Button::create();
	newBtn->setText("New Game");
	this->container->add(newBtn);
//	g->getGui()->add(this->container);
}