#include "Game.h"
#include <memory>
#include "Entity.h"
#include "EntityPrefabs.h"
#include "RenderManager.h"
#include "ShipRenderer.h"

Game::Game(sf::RenderWindow& window) : window(window)
{
	this->gMap = GameMap();
	this->entities.push_back(EntityPrefabs::playerShip(this, ShipRenderer::SAIL_COLOR::Green));
	this->player = this->entities.back();
	this->addEntity(EntityPrefabs::enemyChasingShip(this, { 200, 200 }, ShipRenderer::SAIL_COLOR::Black));
}

void Game::update(double delta)
{
	// Update all entities
	for (size_t i = 0; i < this->entities.size(); i++) {
		auto e = this->entities[i];
		if (e->controller != nullptr) {
			e->controller->update(delta);
		}
	}
	// Check for/resolve collision
	for (size_t i = 0; i < this->entities.size(); i++) {
		auto eOne = this->entities[i];
		for (size_t x = i + 1; x < this->entities.size(); x++) {
			auto eTwo = this->entities[x];
			// Ensure they both have colliders
			if (eOne->collider != nullptr && eTwo->collider != nullptr) {
				// Check they both have colliders
				if (eOne->collider->checkForCollision(eTwo->collider)) {
					// If they have colliders, call the onCollision event
					if (eOne->controller != nullptr)
						eOne->controller->onCollision(eTwo);
					if (eTwo->controller != nullptr)
						eTwo->controller->onCollision(eOne);
				}
			}
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

std::vector<std::shared_ptr<Entity>> Game::getEntities()
{
	return this->entities;
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