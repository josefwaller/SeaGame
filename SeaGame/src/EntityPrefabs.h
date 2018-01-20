#pragma once
#include <memory>
#include "Game.h"
#include "Entity.h"

// Contain methods to easily create different types of entities
class EntityPrefabs
{
public:
	static std::shared_ptr<Entity> playerShip(Game* g);
	static std::shared_ptr<Entity> cannonBall(Game* g, sf::Vector2f pos, float rot);
};