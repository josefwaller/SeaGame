#include "MainMenu.h"
#include <iostream>
#include <filesystem>
#include "SaveFile.h"
#include "ResourceManager.h"

MainMenu::MainMenu(App* a) : app(a) {
	this->container = tgui::Group::create();
	// Add title text
	auto titleText = tgui::Label::create();
	titleText->setText("Sea Game!");
	titleText->setPosition(
		(tgui::bindWidth(this->container) - tgui::bindWidth(titleText)) / 2,
		tgui::bindHeight(this->container) / 6);
	titleText->setTextSize(50);
	this->container->add(titleText);
	auto vLay = tgui::VerticalLayout::create();
	vLay->setSize(500, 350);
	vLay->setPosition(
		(tgui::bindWidth(this->container) - tgui::bindWidth(vLay)) / 2,
		tgui::bindHeight(this->container) * 2 / 3 - tgui::bindHeight(vLay) / 2
	);
	this->container->add(vLay);
	tgui::Button::Ptr newBtn = tgui::Button::create();
	newBtn->setText("New Game");
	newBtn->connect("clicked", &App::newGame, this->app);
	vLay->add(newBtn);
	tgui::Button::Ptr loadBtn = tgui::Button::create();
	loadBtn->setText("Load");
	const MainMenu* m = this->app->getMainMenu();
	loadBtn->connect("clicked", &MainMenu::showSaveFiles, (MainMenu*)m);
	vLay->add(loadBtn);
}

void MainMenu::show() {
	this->app->getGui()->add(this->container);
}
void MainMenu::hide() {
	this->app->getGui()->remove(this->container);
}
void MainMenu::render(sf::RenderWindow* window) {
	// Draw water behind the buttons
	sf::Sprite waterSprite = ResourceManager::get()->getSprite("tiles", "sea-sea-sea-sea", false);
	for (size_t x = 0; x < window->getSize().x / 64; x++) {
		for (size_t y = 0; y < window->getSize().y / 64; y++) {
			waterSprite.setPosition(64 * x, 64 * y);
			window->draw(waterSprite);
		}
	}
}
void MainMenu::showSaveFiles() {
	// Reset container
	this->container->removeAllWidgets();
	// Add a button for each file
	tgui::VerticalLayout::Ptr layout = tgui::VerticalLayout::create();
	for (const auto entry : std::filesystem::directory_iterator(SaveFile::SAVE_FILE_DIR)) {
		tgui::Button::Ptr b = tgui::Button::create();
		std::string fileName = entry.path().filename().string();
		b->setText(fileName);
		b->connect("clicked", &App::loadGame, this->app, fileName);
		layout->add(b);
	}
	// Add buttons to container
	this->container->add(layout);
}