#pragma once
#include <memory>
#include "Game.h"
#include "Entity.h"

// Contain methods to easily create different types of entities
class EntityPrefabs
{
public:
	static std::shared_ptr<Entity> playerShip(Game* g);
};