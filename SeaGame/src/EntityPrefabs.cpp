#include "EntityPrefabs.h"
#include "ShipRenderer.h"
#include "SpriteRenderer.h"
#include "PlayerShipController.h"
#include "ChasingShipController.h"
#include "CannonBallController.h"
#include "BoxCollider.h"

std::shared_ptr<Entity> EntityPrefabs::playerShip(Game* g, ShipRenderer::SAIL_COLOR c)
{
	auto ship = EntityPrefabs::ship(g, { 30, 30 }, 0.0f, c);
	ship->controller = std::shared_ptr<ControllerComponent>(new PlayerShipController(ship));
	return ship;
}

std::shared_ptr<Entity> EntityPrefabs::cannonBall(Game* g, sf::Vector2f pos, float rot)
{
	auto ball = std::shared_ptr<Entity>(new Entity(g));
	ball->renderer = std::shared_ptr<RenderComponent>(new SpriteRenderer(
		ball,
		"ships",
		"cannonBall.png",
		RenderManager::INDEX_CANNONBALLS));
	ball->controller = std::shared_ptr<ControllerComponent>(new CannonBallController(ball));
	ball->transform = std::shared_ptr<TransformComponent>(new TransformComponent(ball, pos, rot));
	return ball;
}

std::shared_ptr<Entity> EntityPrefabs::enemyChasingShip(Game* g, sf::Vector2f pos, ShipRenderer::SAIL_COLOR c)
{
	auto ship = EntityPrefabs::ship(g, pos, 0.0f, c);
	ship->controller = std::shared_ptr<ControllerComponent>(new ChasingShipController(ship));
	return ship;
}

std::shared_ptr<Entity> EntityPrefabs::ship(Game* g, sf::Vector2f pos, float rot, ShipRenderer::SAIL_COLOR c)
{
	auto ship = std::shared_ptr<Entity>(new Entity(g));
	ship->transform = std::shared_ptr<TransformComponent>(new TransformComponent(ship, pos, rot));
	ship->collider = std::shared_ptr<ColliderComponent>(new BoxCollider(ship, { -60, -30 }, 120, 60));
	ship->renderer = std::shared_ptr<RenderComponent>(new ShipRenderer(ship, c));
	return ship;
}