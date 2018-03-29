#include "EntityPrefabs.h"
#include "ShipRenderer.h"
#include "SpriteRenderer.h"
#include "PlayerShipController.h"
#include "ChasingShipController.h"
#include "CannonBallController.h"
#include "AnimationRenderer.h"
#include "ResourceManager.h"
#include "SimpleHealth.h"
#include "ShipHealth.h"
#include "BasicTransform.h"
#include "Box2dTransform.h"

std::shared_ptr<Entity> EntityPrefabs::playerShip(Game* g, ShipRenderer::SAIL_COLOR c)
{
	auto ship = EntityPrefabs::ship(g, { 30, 30 }, 0.0f, c);
	ship->controller = std::shared_ptr<ControllerComponent>(new PlayerShipController(ship));
	return ship;
}

std::shared_ptr<Entity> EntityPrefabs::cannonBall(Game* g, std::weak_ptr<Entity> spawner, sf::Vector2f pos, float rot)
{
	auto ball = std::shared_ptr<Entity>(new Entity(g));
	ball->renderer = std::shared_ptr<RenderComponent>(new SpriteRenderer(
		ball,
		"ships",
		"cannonBall.png",
		RenderManager::INDEX_CANNONBALLS));
	ball->controller = std::shared_ptr<ControllerComponent>(new CannonBallController(ball, spawner));
	ball->transform = std::shared_ptr<TransformComponent>(new BasicTransform(ball, pos, rot));
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
	// Create a ship definition for the new ship
	b2BodyDef shipDef;
	shipDef.type = b2_dynamicBody;
	shipDef.position.Set(pos.x, pos.y);
	shipDef.angle = rot;
	// Create a new fixture and add it to the definition
	b2FixtureDef shipFixture;
	b2PolygonShape shipShape; // lol
	b2Vec2 shapeDimension = { 20, 20 };
	shipShape.SetAsBox(shapeDimension.x, shapeDimension.y);
	shipFixture.shape = &shipShape;
	shipFixture.density = 100 / (4 * shapeDimension.x * shapeDimension.y);
	ship->transform = std::shared_ptr<TransformComponent>(new Box2dTransform(ship, &shipDef, { shipFixture }));
	ship->renderer = std::shared_ptr<RenderComponent>(new ShipRenderer(ship, c));
	ship->health = std::shared_ptr<HealthComponent>(new ShipHealth(ship));
	return ship;
}
std::shared_ptr<Entity> EntityPrefabs::explosion(Game* g, sf::Vector2f pos)
{
	auto ex = std::shared_ptr<Entity>(new Entity(g));
	ex->transform = std::shared_ptr<TransformComponent>(new BasicTransform(ex, pos, 0.0f));
	ex->renderer = std::shared_ptr<RenderComponent>(new AnimationRenderer(
		ex,
		{
			ResourceManager::get()->getSprite("ships", "explosion3.png", true),
			ResourceManager::get()->getSprite("ships", "explosion2.png", true),
			ResourceManager::get()->getSprite("ships", "explosion1.png", true)
		},
		100,
		RenderManager::INDEX_EFFECT,
		false));
	return ex;
}