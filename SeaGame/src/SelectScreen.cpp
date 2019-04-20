#include "Game.h"
#include "SelectScreen.h"
#include "Entity.h"
#include "ClickComponent.h"
#include <memory>
#include <vector>
#include <TGUI/Widgets/Label.hpp>

SelectScreen::SelectScreen(Game* g) {
	this->game = g;
	auto l = tgui::Label::create();
	l->setText("Select an Entity");
	l->setPosition(
		(tgui::bindWidth(g->getGui()) - tgui::bindWidth(l)) / 2,
		100
	);
	l->setTextSize(20);
	l->getRenderer()->setTextColor(tgui::Color::Black);
	this->label = l;
}
void SelectScreen::show(tgui::Container::Ptr c) {
	c->add(this->label);
}
void SelectScreen::hide(tgui::Container::Ptr c) {
	c->remove(this->label);
}
void SelectScreen::setCallback(std::function<void(std::shared_ptr<Entity>)> c) {
	this->callback = c;
}
void SelectScreen::onClick(sf::Vector2f pos) {
	for (auto e : this->game->getEntities()) {
		if (e->components.click->checkForClick(pos)) {
			this->callback(e);
			this->game->getHud()->hideSelect();
		}
	}
}