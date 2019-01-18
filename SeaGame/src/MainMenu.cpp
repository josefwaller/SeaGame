#include "MainMenu.h"
#include <iostream>
#include <filesystem>
#include "SaveFile.h"

MainMenu::MainMenu(App* a) : app(a) {
	this->container = tgui::VerticalLayout::create();
	tgui::Button::Ptr loadBtn = tgui::Button::create();
	loadBtn->setText("Load");
	const MainMenu* m = this->app->getMainMenu();
	loadBtn->connect("clicked", &MainMenu::showSaveFiles, (MainMenu*)m);
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
void MainMenu::showSaveFiles() {
	// Reset container
	this->container->removeAllWidgets();
	// Add a button for each file
	tgui::VerticalLayout::Ptr layout = tgui::VerticalLayout::create();
	for (const auto entry : std::filesystem::directory_iterator(SaveFile::SAVE_FILE_DIR)) {
		tgui::Button::Ptr b = tgui::Button::create();
		std::string fileName = entry.path().string();
		b->setText(fileName);
		b->connect("clicked", &App::loadGame, this->app, fileName);
		layout->add(b);
	}
	// Add buttons to container
	this->container->add(layout);
}