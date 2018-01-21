#include "Game.h"
#include <memory>
#include "Entity.h"
#include "EntityPrefabs.h"
#include "RenderManager.h"

Game::Game(sf::RenderWindow& window) : window(window)
{
	this->gMap = GameMap();
	this->entities.push_back(EntityPrefabs::playerShip(this));
	this->player = this->entities.back();
}

void Game::update(double delta)
{
	for (size_t i = 0; i < this->entities.size(); i++) {
		auto e = this->entities[i];
		if (e->controller != nullptr) {
			e->controller->update(delta);
		}
	}
}

void Game::render()
{
	RenderManager r;
	this->gMap.render(r);
	for (size_t i = 0; i < this->entities.size(); i++) {
		auto e = this->entities[i];
		if (e->renderer != nullptr) {
			e->renderer->render(r);
		}
	}
	r.render(this->window);
	r.reset();
}

void Game::addEntity(std::shared_ptr<Entity> newEnt)
{
	this->entities.push_back(newEnt);
}

sf::Vector2f Game::getMouseCoords()
{
	return (sf::Vector2f)sf::Mouse::getPosition(this->window);
}

std::shared_ptr<Entity> Game::getPlayer()
{
	return this->player.lock();
}