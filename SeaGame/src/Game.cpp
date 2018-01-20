#include "Game.h"
#include <memory>
#include "Entity.h"
#include "EntityPrefabs.h"

Game::Game()
{
	this->gMap = GameMap();
	this->entities.push_back(EntityPrefabs::playerShip(this));
}

void Game::update(double delta)
{
	this->entities[0]->controller->update(delta);
}

void Game::render(sf::RenderWindow& w)
{
	this->gMap.render(w);
	this->entities[0]->renderer->render(w);
}