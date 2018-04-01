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
	b2BodyDef ballDef;
	ballDef.type = b2_dynamicBody;
	ballDef.position.Set(pos.x, pos.y);
	b2FixtureDef ballFixture;
	b2CircleShape ballShape;
	b2CircleShape b = b2CircleShape();
	ballShape.m_radius = 5;
	ballShape.m_p = { -5.0f, -5.0f };
	ballFixture.shape = &ballShape;
	ballFixture.isSensor = true;
	ball->transform = std::shared_ptr<Box2dTransform>(new Box2dTransform(ball, &ballDef, { ballFixture }));
	ball->physics = std::shared_ptr<PhysicsComponent>(new PhysicsComponent(ball));
	ball->controller = std::shared_ptr<ControllerComponent>(new CannonBallController(ball, rot, spawner));
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
	// Create a ship definition for the new ship
	b2BodyDef shipDef;
	shipDef.type = b2_dynamicBody;
	shipDef.position.Set(pos.x, pos.y);
	shipDef.angle = rot;
	// Create a new fixture and add it to the definition
	b2FixtureDef shipFixture;
	// Create shape for the fixture to have
	b2PolygonShape shipShape; // lol
	// Set the shape to a ship-like pentagon
	b2Vec2 verts[5];
	verts[0].Set(-50, -20);
	verts[1].Set(30, -20);
	verts[2].Set(50, 0);
	verts[3].Set(30, 20);
	verts[4].Set(-50, 20);
	shipShape.Set(verts, 5);
	shipFixture.shape = &shipShape;
	// PhysicsComponent will calculate the volume/mass itself, so this value doesn't really matter
	shipFixture.density = 100;
	// Create the actualy ship entity
	auto ship = std::shared_ptr<Entity>(new Entity(g));
	ship->transform = std::shared_ptr<TransformComponent>(new Box2dTransform(ship, &shipDef, { shipFixture }));
	ship->renderer = std::shared_ptr<RenderComponent>(new ShipRenderer(ship, c));
	ship->physics = std::shared_ptr<PhysicsComponent>(new PhysicsComponent(ship));
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