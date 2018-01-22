#include "EntityPrefabs.h"
#include "ShipRenderer.h"
#include "SpriteRenderer.h"
#include "PlayerShipController.h"
#include "ChasingShipController.h"
#include "CannonBallController.h"

std::shared_ptr<Entity> EntityPrefabs::playerShip(Game* g, ShipRenderer::SAIL_COLOR c)
{
	auto ship = std::shared_ptr<Entity>(new Entity(g));
	ship->renderer = std::unique_ptr<RenderComponent>(new ShipRenderer(ship, c));
	ship->transform = std::shared_ptr<TransformComponent>(new TransformComponent(ship, { 30, 30 }, (float)(M_PI / 6)));
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
	auto ship = std::shared_ptr<Entity>(new Entity(g));
	ship->renderer = std::shared_ptr<RenderComponent>(new ShipRenderer(ship, c));
	ship->transform = std::shared_ptr<TransformComponent>(new TransformComponent(ship, pos, 0.0f));
	ship->controller = std::shared_ptr<ControllerComponent>(new ChasingShipController(ship));
	return ship;
}