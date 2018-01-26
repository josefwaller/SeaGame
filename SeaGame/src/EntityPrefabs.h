#pragma once
#include <memory>
#include "Game.h"
#include "Entity.h"
#include "ShipRenderer.h"

// Contain methods to easily create different types of entities
class EntityPrefabs
{
public:
	static std::shared_ptr<Entity> playerShip(Game* g, ShipRenderer::SAIL_COLOR c);
	static std::shared_ptr<Entity> cannonBall(Game* g, sf::Vector2f pos, float rot);
	static std::shared_ptr<Entity> enemyChasingShip(Game* g, sf::Vector2f pos, ShipRenderer::SAIL_COLOR c);

private:
	static std::shared_ptr<Entity> ship(Game* g, sf::Vector2f pos, float rot, ShipRenderer::SAIL_COLOR c);
};