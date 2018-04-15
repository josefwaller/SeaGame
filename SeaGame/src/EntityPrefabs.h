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
	static std::shared_ptr<Entity> cannonBall(Game* g, std::weak_ptr<Entity> spawner, sf::Vector2f pos, float rot);
	static std::shared_ptr<Entity> enemyChasingShip(Game* g, sf::Vector2f pos, ShipRenderer::SAIL_COLOR c);
	static std::shared_ptr<Entity> explosion(Game* g, sf::Vector2f pos);
	static std::shared_ptr<Entity> militaryBase(Game* g, sf::Vector2i pos);
private:
	static std::shared_ptr<Entity> ship(Game* g, sf::Vector2f pos, float rot, ShipRenderer::SAIL_COLOR c);
	static std::shared_ptr<Entity> base(Game* g, sf::Vector2i pos);
};