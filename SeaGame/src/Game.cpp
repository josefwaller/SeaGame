#include "Game.h"
#include <memory>
#include "Entity.h"
#include "EntityPrefabs.h"
#include "RenderManager.h"
#include "ShipRenderer.h"
#include "SimpleCollisionListener.h"

Game::Game(sf::RenderWindow& window, tgui::Gui& gui) : window(window), gui(gui)
{
	// Create world and make gravity 0, since it is top down
	this->world = std::shared_ptr<b2World>(new b2World({ 0.0f, 0.0f }));
	this->listener = SimpleCollisionListener();
	this->world->SetContactListener(&this->listener);
	// Add entities
	this->entities.push_back(EntityPrefabs::playerShip(this, ShipRenderer::SAIL_COLOR::Blue));
	this->player = this->entities.back();
	this->player.lock()->inventory->addItems(GameResource::Gold, 300);
	// Create GameMap
	this->gMap = GameMap(this);
	//this->addEntity(EntityPrefabs::enemyChasingShip(this, { 200, 200 }, ShipRenderer::SAIL_COLOR::Black));
	// Add a base
	this->entities.push_back(EntityPrefabs::miningBase(this, { 5, 5 }));
	auto b1 = this->entities.back();
	this->entities.push_back(EntityPrefabs::miningBase(this, { 20, 5 }));
	auto b2 = this->entities.back();
	this->entities.push_back(EntityPrefabs::ferryShip(this, { 0.0f, 0.0f }, b1, b2));
	// Add the build button
	this->buildBtn = tgui::Button::create();
	this->buildBtn->setText("Build");
	this->gui.add(this->buildBtn);
	this->buildBtn->connect("clicked", &Game::toggleBuildButtons, this);
	this->isBuilding = false;
	float width = 0.0f;
	float height = this->buildBtn->getFullSize().y;
	this->buildThingsBtns = {
		tgui::Button::create(),
		tgui::Button::create()
	};
	this->buildThingsBtns[0]->setText("Generation Base");
	this->buildThingsBtns[0]->setPosition({ 0, this->buildBtn->getFullSize().y });
	this->buildThingsBtns[0]->connect("clicked", [&](Game* game) {
		game->startBuilding([&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::miningBase(g, (sf::Vector2i)(pos / 64.0f));
		});
	}, this);
	width += this->buildThingsBtns[0]->getFullSize().x;
	this->buildThingsBtns[1]->setText("Military Base");
	this->buildThingsBtns[1]->setPosition({ width, height });
	this->buildThingsBtns[1]->connect("clicked", [&](Game * game) {
		game->startBuilding([&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::militaryBase(g, (sf::Vector2i)(pos / 64.0f));
		});
	}, this);
}

void Game::update(double delta)
{
	// Update all entities
	for (size_t i = 0; i < this->entities.size(); i++) {
		auto e = this->entities[i];
		if (e->controller != nullptr)
			e->controller->update((float)delta);
	}
	// Update world and resolve collisions
	this->world->Step((float) delta, 8, 3);
	// Remove entities
	for (auto it : this->toRemove) {
		this->entities.erase(
			std::remove(this->entities.begin(), this->entities.end(), it.lock()),
			this->entities.end());
	}
}

void Game::render()
{
	// Render Entities
	RenderManager r;
	this->gMap.render(r);
	for (size_t i = 0; i < this->entities.size(); i++) {
		auto e = this->entities[i];
		if (e->renderer != nullptr) {
			e->renderer->render(r);
			e->renderer->renderCollider(r);
		}
	}
	// Render debug info, i.e. hitboxes
	for (auto e : this->entities) {
		if (e->renderer != nullptr) {
			e->renderer->renderCollider(r);
		}
	}
	r.render(this->window);
	r.reset();
	// Render GUI
	this->gui.draw();
}
void Game::handleEvent(sf::Event e) {
	switch (e.type) {
	case sf::Event::MouseButtonPressed:
		if (e.key.code == sf::Mouse::Left) {
			if (!this->isBuilding) {
				for (auto e : this->entities) {
					if (e->inventory != nullptr) {
						e->inventory->checkForOpen();
					}
				}
			}
			else {
				sf::Vector2f mousePos = this->getMouseCoords();
				this->entities.push_back(this->buildFunction(this, mousePos));
				this->isBuilding = false;
			}
		}
	}
}
std::vector<std::shared_ptr<Entity>> Game::getEntities()
{
	return this->entities;
}
void Game::addEntity(std::shared_ptr<Entity> newEnt)
{
	this->entities.push_back(newEnt);
}
void Game::removeEntity(std::weak_ptr<Entity> e)
{
	this->toRemove.push_back(e);
}
sf::Vector2f Game::getMouseCoords()
{
	return (sf::Vector2f)sf::Mouse::getPosition(this->window);
}

std::shared_ptr<Entity> Game::getPlayer()
{
	return this->player.lock();
}

std::weak_ptr<b2World> Game::getWorld()
{
	return this->world;
}
tgui::Gui& Game::getGui() {
	return this->gui;
}
void Game::toggleBuildButtons() {
	for (auto btn : this->buildThingsBtns) {
		this->gui.add(btn);
	}
}
void Game::startBuilding(std::function<std::shared_ptr<Entity>(Game* g, sf::Vector2f pos)> func) {
	this->isBuilding = true;
	this->buildFunction = func;
}