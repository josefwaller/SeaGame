#include "Game.h"
#include <memory>
#include "Entity.h"
#include "SpriteRenderer.h"

Game::Game()
{
	this->gMap = GameMap();
	this->ship = Entity();
	this->ship.renderer = std::unique_ptr<RenderComponent>(new SpriteRenderer(this->ship, "ships", "ship (10).png"));
}

void Game::update(unsigned int delta)
{

}

void Game::render(sf::RenderWindow& w)
{
	this->gMap.render(w);
	this->ship.renderer->render(w);
}