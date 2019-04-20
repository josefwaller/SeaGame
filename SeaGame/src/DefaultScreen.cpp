#include "DefaultScreen.h"
#include "GameHud.h"
#include "Game.h"
#include "Entity.h"
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/SignalImpl.hpp>
#include <TGUI/Widgets/ChildWindow.hpp>
#include <TGUI/Widgets/VerticalLayout.hpp>
// Player health display
#include "HealthComponent.h"
// Checking for click/running click code
#include "ClickComponent.h"
#include "ControllerComponent.h"
#include "GuiComponent.h"

const float DefaultScreen::ANNOUNCEMENT_WIDTH = 400.0f;
const float DefaultScreen::ANNOUNCEMENT_ITEM_HEIGHT = 60.0f;

DefaultScreen::DefaultScreen(Game* g) {
	this->game = g;
	this->container = tgui::Group::create();
	// Add the build button
	auto buildButton = tgui::Button::create();
	buildButton->setText("Build");
	buildButton->connect("clicked", &GameHud::showBuild, this->game->getHud());
	this->container->add(buildButton);
	// Add research button
	auto researchButton = tgui::Button::create();
	researchButton->setText("Research");
	researchButton->setPosition({ 500.0f, 0 });
	researchButton->connect("clicked", [&](Game* g) {
		g->getHud()->showResearch();
	}, this->game);
	this->container->add(researchButton);
	// Add money display
	this->moneyDisplay = tgui::Label::create();
	this->moneyDisplay->setPosition({ this->game->getWindow()->getSize().x - 300, 0 });
	this->container->add(this->moneyDisplay);
	// Add player health display
	this->playerHealth = tgui::ProgressBar::create();
	this->playerHealth->getRenderer()->setFillColor(tgui::Color(255, 0, 0, 255));
	this->playerHealth->setMaximum(100);
	this->playerHealth->setPosition({
		0,
		this->game->getWindow()->getSize().y - this->playerHealth->getFullSize().y
	});
	this->container->add(this->playerHealth);
	auto aC = tgui::VerticalLayout::create();
	aC->setPosition({ tgui::bindWidth(this->game->getGui()) - 400, 500 });
	this->announcementContainer = aC;
	this->container->add(this->announcementContainer);
	// Create entity panel
	auto eP = tgui::ChildWindow::create();
	eP->setSize(tgui::bindWidth(this->game->getGui()), 250);
	eP->setPosition(
		0,
		tgui::bindHeight(this->game->getGui()) - tgui::bindHeight(eP)
	);
	eP->setResizable(false);
	eP->setPositionLocked(true);
	this->entityPanel = eP;
	// Add menu button
	auto menuButton = tgui::Button::create();
	menuButton->setText("Menu");
	menuButton->setPosition(400, 0);
	menuButton->connect("clicked", &GameHud::showPause, this->game->getHud());
	this->container->add(menuButton);

}
void DefaultScreen::update() {
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
}
void DefaultScreen::onClick(sf::Vector2f pos) {
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
void DefaultScreen::showEntity(std::weak_ptr<Entity> e) {
	if (e.lock()) {
		if (auto g = e.lock()->components.gui) {
			// Remove anything in the entity panel
			this->entityPanel->removeAllWidgets();
			// Add the entity's gui
			g->show(this->entityPanel);
			// Add the entity panel if it is not included already
			if (!this->container->get("entityPanel")) {
				this->container->add(this->entityPanel, "entityPanel");
			}
		}
	}
}
void DefaultScreen::addAnnoucnement(std::string announcementStr) {
	tgui::TextBox::Ptr p = tgui::TextBox::create();
	p->setText(announcementStr);
	p->getRenderer()->setTextColor(tgui::Color(rand() % 255, rand() % 255, rand() % 255, 255));
	this->announcementContainer->add(p);
	this->announcements.push({ p, sf::Clock() });
	this->announcementContainer->setSize(
		ANNOUNCEMENT_WIDTH,
		this->announcements.size() * ANNOUNCEMENT_ITEM_HEIGHT);
}
void DefaultScreen::show(tgui::Container::Ptr c) {
	c->add(container);
}
void DefaultScreen::hide(tgui::Container::Ptr c) {
	c->remove(container);
}