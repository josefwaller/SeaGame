#include "GameHud.h"
#include "GenerationBaseController.h"
#include "EntityPrefabs.h"
#include "CraftRecipes.h"
#include "TechTree.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "GameResource.h"
#include "BasicTransform.h"
#include "InventoryComponent.h"
#include "ClickComponent.h"
#include "ResearchScreen.h"
#include "Entity.h"
#include <TGUI/SignalImpl.hpp>

const float GameHud::ANNOUNCEMENT_WIDTH = 400.0f;
const float GameHud::ANNOUNCEMENT_ITEM_HEIGHT = 60.0f;

GameHud::GameHud() {
}
GameHud::GameHud(Game* g): researchScreen(g), buildScreen(g) {
	this->game = g;
	// Set click state to nothing initially
	this->currentState = State::Nothing;
	// Add the build button
	this->buildButton = tgui::Button::create();
	this->buildButton->setText("Build");
	this->buildButton->connect("clicked", &GameHud::showBuild, game->getHud());
	this->game->getGui()->add(this->buildButton);
	// Add research button
	this->researchButton = tgui::Button::create();
	this->researchButton->setText("Research");
	this->researchButton->setPosition({ 500.0f, 0 });
	this->researchButton->connect("clicked", [&](Game* g) {
		g->getHud()->showResearch();
	}, this->game);
	this->game->getGui()->add(this->researchButton);
	// Add money display
	this->moneyDisplay = tgui::Label::create();
	this->moneyDisplay->setPosition({ this->game->getWindow()->getSize().x - 300, 0 });
	this->game->getGui()->add(this->moneyDisplay);
	// Add player health display
	this->playerHealth = tgui::ProgressBar::create();
	this->playerHealth->getRenderer()->setFillColor(tgui::Color(255, 0, 0, 255));
	this->playerHealth->setMaximum(100);
	this->playerHealth->setPosition({ 0, this->game->getWindow()->getSize().y - this->playerHealth->getFullSize().y });
	this->game->getGui()->add(this->playerHealth);
	this->announcementContainer = tgui::VerticalLayout::create();
	this->announcementContainer->setPosition({ this->game->getGui()->getSize().x - 400, 500 });
	this->game->getGui()->add(this->announcementContainer);
	// Create entity panel
	this->entityPanel = tgui::ChildWindow::create();
	this->entityPanel->setSize(tgui::bindWidth(this->game->getGui()), 250);
	this->entityPanel->setPosition(
		0,
		tgui::bindHeight(this->game->getGui()) - tgui::bindHeight(this->entityPanel)
	);
	this->entityPanel->setResizable(false);
	this->entityPanel->setPositionLocked(true);
	// Set up state text
	this->stateText = tgui::Label::create();
	this->stateText->setTextSize(30);
	this->stateText->setText("Choose what to build");
	this->stateText->setPosition((this->game->getGui()->getSize() - this->stateText->getSize()).x / 2, 200.0f);
	this->stateText->getRenderer()->setBackgroundColor(tgui::Color::Black);
	this->game->getGui()->add(this->stateText);
	this->stateText->setVisible(false);
	// Add menu
	auto gui = this->game->getGui();
	auto menu = tgui::ChildWindow::create();
	menu->setSize(400, 600);
	menu->setResizable(false);
	menu->setPositionLocked(true);
	menu->setPosition(
		(tgui::bindWidth(gui) - tgui::bindWidth(menu)) / 2,
		(tgui::bindHeight(gui) - tgui::bindHeight(menu)) / 2
	);
	// Add horizontal layout inside
	auto vL = tgui::VerticalLayout::create();
	menu->add(vL);
	// Add save button
	auto saveBtn = tgui::Button::create();
	saveBtn->setText("Save");
	saveBtn->connect("clicked", &Game::save, this->game);
	vL->add(saveBtn);
	// Add quit button
	auto quitBtn = tgui::Button::create();
	quitBtn->setText("Quit");
	quitBtn->connect("clicked", &Game::quitGame, this->game);
	vL->add(quitBtn);
	// Add menu button
	auto menuButton = tgui::Button::create();
	menuButton->setText("Menu");
	menuButton->setPosition(400, 0);
	menuButton->connect("clicked", [](tgui::ChildWindow::Ptr w, tgui::Container::Ptr gui) {
		gui->add(w);
	}, menu, gui);
	gui->add(menuButton);
}
void GameHud::update() {
	// Update money display
	this->moneyDisplay->setText("$" + std::to_string(this->game->getMoney()));
	// Update player health
	if (this->game->getPlayer())
		if (this->game->getPlayer()->components.health)
			this->playerHealth->setValue(this->game->getPlayer()->components.health->getHealth());
	// Update announcements
	while (!this->announcements.empty()
		&& this->announcements.front().second.getElapsedTime().asMilliseconds() >= 3000.0) {
		this->announcementContainer->remove(this->announcements.front().first);
		this->announcements.pop();
		this->announcementContainer->setSize(
			this->announcementContainer->getSize().x,
			this->announcements.size() * ANNOUNCEMENT_ITEM_HEIGHT);
	}
	// Update the research tech tree
	this->researchScreen.update();
	// Update the build screen
	// this->buildScreen.update();
}
void GameHud::render(RenderManager& rm) {
	switch (this->currentState) {
	case State::Research:
		this->researchScreen.render(rm);
		break;
	case State::Building:
		this->buildScreen.render(rm);
		break;
	}
}
void GameHud::showResearch() {
	this->currentState = State::Research;
	this->researchScreen.show(this->game->getGui());
}
void GameHud::hideResearch() {
	this->currentState = State::Nothing;
	this->researchScreen.hide(this->game->getGui());
}
void GameHud::showBuild() {
	this->currentState = State::Building;
	this->buildScreen.show(this->game->getGui());
}
void GameHud::hideBuild() {
	this->currentState = State::Nothing;
	this->buildScreen.hide(this->game->getGui());
}
void GameHud::updateBuild() {
	this->buildScreen.update();
}
void GameHud::transferItems(std::weak_ptr<Entity> e, GameResource res, unsigned int amount) {
	this->selectCallback = std::bind([&](
		std::weak_ptr<Entity> to,
		std::weak_ptr<Entity> from,
		GameResource res,
		unsigned int amount) {
		// Ensure both entities still exist
		if (to.lock() && from.lock()) {
			// Remove resources
			from.lock()->components.inventory->removeItems(res, amount);
			// Add resources
			to.lock()->components.inventory->addItems(res, amount);
		}
	}, std::placeholders::_1, e, res, amount);
	this->currentState = State::Selecting;
}
void GameHud::onClick(sf::Vector2f pos) {
	if (this->currentState == State::Nothing) {
		for (auto e : this->game->getEntities()) {
			if (e->components.click != nullptr) {
				if (e->components.click->checkForClick(pos)) {
					if (e->components.controller != nullptr) {
						e->components.controller->onClick();
					}
				}
			}
		}
	}
	else if (this->currentState == State::Building) {
		this->buildScreen.onClick(pos);
	}
	else if (this->currentState == State::Selecting) {
		for (auto e : this->game->getEntities()) {
			if (e->components.click) {
				if (e->components.click->checkForClick(pos)) {
					this->selectCallback(e);
					this->currentState = State::Nothing;
					this->stateText->setVisible(false);
				}
			}
		}
	}
}
void GameHud::selectEntity(std::function<void(std::weak_ptr<Entity> entity)> callback) {
	this->selectCallback = callback;
	this->currentState = State::Selecting;
	this->stateText->setText("Select an entity");
	this->stateText->setVisible(true);
}
void GameHud::addAnnouncement(std::string announcement) {
	tgui::TextBox::Ptr p = tgui::TextBox::create();
	p->setText(announcement);
	p->getRenderer()->setTextColor(tgui::Color(rand() % 255, rand() % 255, rand() % 255, 255));
	this->announcementContainer->add(p);
	this->announcements.push({ p, sf::Clock() });
	this->announcementContainer->setSize(
		ANNOUNCEMENT_WIDTH,
		this->announcements.size() * ANNOUNCEMENT_ITEM_HEIGHT);
}
tgui::ChildWindow::Ptr GameHud::getEntityPanel() {
	return this->entityPanel;
}