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

GameHud::GameHud(Game* g): researchScreen(g), buildScreen(g), pauseScreen(g), defaultScreen(g) {
	this->game = g;
	// Set click state to nothing initially
	this->currentState = State::Nothing;
	this->showDefault();
}
void GameHud::update() {
	// Update the research tech tree
	switch (this->currentState) {
	case State::Research:
		this->researchScreen.update();
		break;
	case State::Nothing:
		this->defaultScreen.update();
		break;
	}
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
void GameHud::hideCurrent() {
	switch (this->currentState) {
	case State::Research:
		this->hideResearch();
		break;
	case State::Building:
		this->hideBuild();
		break;
	case State::Paused:
		this->hidePause();
		break;
	case State::Nothing:
		this->hideDefault();
	}
}
void GameHud::showResearch() {
	this->hideCurrent();
	this->currentState = State::Research;
	this->researchScreen.show(this->game->getGui());
}
void GameHud::hideResearch() {
	this->currentState = State::Nothing;
	this->researchScreen.hide(this->game->getGui());
	this->showDefault();
}
void GameHud::showBuild() {
	this->hideCurrent();
	this->currentState = State::Building;
	this->buildScreen.show(this->game->getGui());
}
void GameHud::hideBuild() {
	this->currentState = State::Nothing;
	this->buildScreen.hide(this->game->getGui());
	this->showDefault();
}
void GameHud::updateBuild() {
	this->buildScreen.update();
}
void GameHud::showPause() {
	if (this->currentState != State::Paused) {
		this->hideCurrent();
		this->currentState = State::Paused;
		this->pauseScreen.show(this->game->getGui());
	}
}
void GameHud::hidePause() {
	if (this->currentState == State::Paused) {
		this->currentState = State::Nothing;
		this->pauseScreen.hide(this->game->getGui());
	}
	this->showDefault();
}
void GameHud::showDefault() {
	this->defaultScreen.show(this->game->getGui());
}
void GameHud::hideDefault() {
	this->defaultScreen.hide(this->game->getGui());
}
void GameHud::showEntity(std::weak_ptr<Entity> e) {
	this->defaultScreen.showEntity(e);
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
		this->defaultScreen.onClick(pos);
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
				}
			}
		}
	}
}
void GameHud::selectEntity(std::function<void(std::weak_ptr<Entity> entity)> callback) {
	this->selectCallback = callback;
	this->currentState = State::Selecting;
}
void GameHud::addAnnouncement(std::string announcement) {
	this->defaultScreen.addAnnoucnement(announcement);
}