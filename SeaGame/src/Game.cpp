#include "Game.h"
#include <memory>
#include "Entity.h"
#include "SpriteRenderer.h"
#include "PlayerShipController.h"
#define _USE_MATH_DEFINES
#include <math.h>

Game::Game()
{
	this->gMap = GameMap();
	this->ship = Entity();
	this->ship.renderer = std::unique_ptr<RenderComponent>(new SpriteRenderer(this->ship, "ships", "ship (10).png"));
	this->ship.transform = std::shared_ptr<TransformComponent>(new TransformComponent(this->ship, { 30, 30 }, M_PI / 6));
	this->ship.controller = std::shared_ptr<ControllerComponent>(new PlayerShipController(this->ship));
}

void Game::update(double delta)
{
	this->ship.controller->update(delta);
}

void Game::render(sf::RenderWindow& w)
{
	this->gMap.render(w);
	this->ship.renderer->render(w);
}