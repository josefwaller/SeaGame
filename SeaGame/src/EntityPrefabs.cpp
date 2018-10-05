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
#include "BaseRenderer.h"
#include "MilitaryBaseRenderer.h"
#include "MilitaryBaseController.h"
#include "MiningBaseController.h"
#include "FerryShipController.h"
#include "GuiComponent.h"
#include "Box2dClick.h"
#include "CityController.h"
#include "CityRenderer.h"
#include "MiningBaseRenderer.h"
#include "PirateBaseRenderer.h"
#include "PirateBaseController.h"

std::shared_ptr<Entity> EntityPrefabs::playerShip(Game* g, sf::Vector2f position)
{
	auto ship = EntityPrefabs::ship(g, position, 0.0f, ShipRenderer::SAIL_COLOR::Blue);
	ship->team = 0;
	ship->type = EntityType::Player;
	ship->controller = std::shared_ptr<ControllerComponent>(new PlayerShipController(ship));
	return ship;
}

std::shared_ptr<Entity> EntityPrefabs::cannonBall(Game* g, std::weak_ptr<Entity> spawner, sf::Vector2f pos, float rot)
{
	auto ball = std::shared_ptr<Entity>(new Entity(g));
	ball->tag = EntityTag::Cannonball;
	ball->renderer = std::shared_ptr<RenderComponent>(new SpriteRenderer(
		ball,
		"ships",
		"cannonBall.png",
		RenderManager::INDEX_CANNONBALLS));
	// Set up Box2d definition
	b2BodyDef ballDef;
	ballDef.type = b2_dynamicBody;
	ballDef.position.Set(pos.x, pos.y);
	b2FixtureDef ballFixture;
	b2CircleShape ballShape;
	// Set up shape for the hitbox
	ballShape.m_radius = 5;
	ballShape.m_p = { -5.0f, -5.0f };
	ballFixture.shape = &ballShape;
	// Make sensor so that the ball creates an explosion when it hits something
	ballFixture.isSensor = true;
	// Set ball team to it's creator's team
	ball->team = spawner.lock()->team;
	ball->transform = std::shared_ptr<Box2dTransform>(new Box2dTransform(ball, &ballDef, { ballFixture }));
	ball->physics = std::shared_ptr<PhysicsComponent>(new PhysicsComponent(ball));
	ball->controller = std::shared_ptr<ControllerComponent>(new CannonBallController(ball, rot));
	return ball;
}

std::shared_ptr<Entity> EntityPrefabs::enemyChasingShip(Game* g, sf::Vector2f pos)
{
	auto ship = EntityPrefabs::ship(g, pos, 0.0f, ShipRenderer::SAIL_COLOR::Black);
	ship->team = 1;
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
	ship->tag = EntityTag::Ship;
	ship->transform = std::shared_ptr<TransformComponent>(new Box2dTransform(ship, &shipDef, { shipFixture }));
	ship->renderer = std::shared_ptr<RenderComponent>(new ShipRenderer(ship, c));
	ship->physics = std::shared_ptr<PhysicsComponent>(new PhysicsComponent(ship));
	ship->health = std::shared_ptr<HealthComponent>(new ShipHealth(ship));
	ship->inventory = std::shared_ptr<InventoryComponent>(new InventoryComponent(ship));
	ship->gui = std::shared_ptr<GuiComponent>(new GuiComponent(ship));
	ship->click = std::shared_ptr<ClickComponent>(new Box2dClick(ship));
	return ship;
}
std::shared_ptr<Entity> EntityPrefabs::militaryBase(Game* g, sf::Vector2i pos)
{
	auto b = base(g, pos);
	b->controller = std::shared_ptr<ControllerComponent>(new MilitaryBaseController(b));
	b->renderer = std::shared_ptr<RenderComponent>(new MilitaryBaseRenderer(b));
	// TODO: Make team a parameter
	b->team = 1;
	return b;
}
std::shared_ptr<Entity> EntityPrefabs::miningBase(Game* g, sf::Vector2i pos) {
	auto b = base(g, pos);
	b->controller = std::shared_ptr<ControllerComponent>(new MiningBaseController(b, GameResource::Stone));
	b->renderer = std::shared_ptr<RenderComponent>(new MiningBaseRenderer(b, GameResource::Stone));
	b->team = 1;
	return b;
}
std::shared_ptr<Entity> EntityPrefabs::base(Game* g, sf::Vector2i pos)
{
	b2BodyDef baseDef;
	baseDef.type = b2_staticBody;
	baseDef.position = b2Vec2(pos.x * 64, pos.y * 64);
	baseDef.angle = 0.0f;
	b2FixtureDef baseFixture;
	b2PolygonShape baseShape;
	b2Vec2 verts[4];
	verts[0] = b2Vec2(0, 0);
	verts[1] = b2Vec2(0, 3 * 64);
	verts[2] = b2Vec2(3 * 64, 3 * 64);
	verts[3] = b2Vec2(3 * 64, 0);
	baseShape.Set(verts, 4);
	baseFixture.shape = &baseShape;

	auto base = std::shared_ptr<Entity>(new Entity(g));
	base->tag = EntityTag::Base;
	base->transform = std::shared_ptr<TransformComponent>(new Box2dTransform(base, &baseDef, { baseFixture }));
	base->renderer = std::shared_ptr<RenderComponent>(new BaseRenderer(base));
	base->gui = std::shared_ptr<GuiComponent>(new GuiComponent(base));
	base->click = std::shared_ptr<ClickComponent>(new Box2dClick(base));
	base->inventory = std::shared_ptr<InventoryComponent>(new InventoryComponent(base));
	return base;
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
std::shared_ptr<Entity> EntityPrefabs::ferryShip(Game* g, sf::Vector2f pos, std::weak_ptr<Entity> from, std::weak_ptr<Entity> to) {
	auto ship = EntityPrefabs::ship(g, pos, 0.0f, ShipRenderer::SAIL_COLOR::Red);
	ship->controller = std::shared_ptr<ControllerComponent>(new FerryShipController(ship, from, to));
	ship->gui = std::shared_ptr<GuiComponent>(new GuiComponent(ship));
	return ship;
}
std::shared_ptr<Entity> EntityPrefabs::city(Game* g, sf::Vector2i pos) {
	auto city = EntityPrefabs::base(g, pos);
	city->controller = std::shared_ptr<ControllerComponent>(new CityController(city));
	city->renderer = std::shared_ptr<RenderComponent>(new CityRenderer(city));
	return city;
}
std::shared_ptr<Entity> EntityPrefabs::forestryBase(Game* g, sf::Vector2i pos) {
	auto base = EntityPrefabs::base(g, pos);
	base->renderer = std::shared_ptr<RenderComponent>(new MiningBaseRenderer(base, GameResource::Wood));
	base->controller = std::shared_ptr<ControllerComponent>(new MiningBaseController(base, GameResource::Wood));
	return base;
}
std::shared_ptr<Entity> EntityPrefabs::pirateBase(Game* g, sf::Vector2i pos) {
	auto base = EntityPrefabs::base(g, pos);
	base->renderer = std::shared_ptr<RenderComponent>(new PirateBaseRenderer(base));
	base->controller = std::shared_ptr<ControllerComponent>(new PirateBaseController(base));
	return base;
}

std::shared_ptr<Entity> EntityPrefabs::getEntityFromSaveData(Game* g, std::map<std::string, std::string> data) {
	EntityType type = (EntityType)std::stoi(data["type"]);
	float x = std::stoi(data["x"]);
	float y = std::stoi(data["y"]);
	switch (type){
	case EntityType::MiningBase:
		return EntityPrefabs::miningBase(g, { (int)x, (int)y });
	case EntityType::ForestryBase:
		return EntityPrefabs::forestryBase(g, { (int)x, (int)y });
	case EntityType::Player:
		return EntityPrefabs::playerShip(g, { x, y });
	}
	return nullptr;
}