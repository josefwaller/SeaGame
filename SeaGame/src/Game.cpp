#include "Game.h"
#include <memory>
#include "Entity.h"
#include "EntityPrefabs.h"
#include "RenderManager.h"
#include "ShipRenderer.h"
#include "SimpleCollisionListener.h"

Game::Game(sf::RenderWindow& window) : window(window)
{
	// Create world and make gravity 0, since it is top down
	this->world = std::shared_ptr<b2World>(new b2World({ 0.0f, 0.0f }));
	this->listener = SimpleCollisionListener();
	this->world->SetContactListener(&this->listener);
	// Add entities
	this->entities.push_back(EntityPrefabs::playerShip(this, ShipRenderer::SAIL_COLOR::Blue));
	this->player = this->entities.back();
	// Create GameMap
	this->gMap = GameMap(this);
	this->addEntity(EntityPrefabs::enemyChasingShip(this, { 200, 200 }, ShipRenderer::SAIL_COLOR::Black));
	// Add a base
	this->entities.push_back(EntityPrefabs::militaryBase(this, { 5, 5 }));
}

void Game::update(double delta)
{
	// Update all entities
	for (size_t i = 0; i < this->entities.size(); i++) {
		auto e = this->entities[i];
		if (e->controller != nullptr) {
			e->controller->update((float)delta);
		}
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