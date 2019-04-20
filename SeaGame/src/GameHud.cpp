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

GameHud::GameHud(Game* g): researchScreen(g), buildScreen(g), pauseScreen(g), defaultScreen(g), selectScreen(g) {
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
		break;
	case State::Selecting:
		this->hideSelect();
		break;
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
void GameHud::showSelect() {
	this->hideDefault();
	this->selectScreen.show(this->game->getGui());
	this->currentState = State::Selecting;
}
void GameHud::hideSelect() {
	this->selectScreen.hide(this->game->getGui());
	this->currentState = State::Nothing;
	this->showDefault();
}
void GameHud::showEntity(std::weak_ptr<Entity> e) {
	this->defaultScreen.showEntity(e);
}
void GameHud::transferItems(std::weak_ptr<Entity> e, GameResource res, unsigned int amount) {
	this->selectScreen.setCallback([e, res, amount](
		std::weak_ptr<Entity> to) {
		// Ensure both entities still exist
		if (to.lock() && e.lock()) {
			// Remove resources
			e.lock()->components.inventory->removeItems(res, amount);
			// Add resources
			to.lock()->components.inventory->addItems(res, amount);
		}
	});
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
		this->selectScreen.onClick(pos);
	}
}
void GameHud::selectEntity(std::function<void(std::weak_ptr<Entity> entity)> callback) {
	this->selectScreen.setCallback(callback);
	this->showSelect();
}
void GameHud::addAnnouncement(std::string announcement) {
	this->defaultScreen.addAnnoucnement(announcement);
}