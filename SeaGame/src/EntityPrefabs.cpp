#include "EntityPrefabs.h"
#include "ShipRenderer.h"
#include "SpriteRenderer.h"
#include "PlayerShipController.h"
#include "ChasingShipController.h"
#include "CannonBallController.h"
#include "AnimationRenderer.h"
#include "ResourceManager.h"
#include "HealthComponent.h"
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
#include "ConversionBaseController.h"

std::shared_ptr<Entity> EntityPrefabs::playerShip(Game* g, sf::Vector2f position)
{
	auto ship = EntityPrefabs::ship(g, position, 0.0f, ShipRenderer::SAIL_COLOR::Blue);
	ship->team = 0;
	ship->type = EntityType::Player;
	ship->components.controller = std::shared_ptr<ControllerComponent>(new PlayerShipController(ship));
	return ship;
}

std::shared_ptr<Entity> EntityPrefabs::cannonBall(Game* g, std::weak_ptr<Entity> spawner, sf::Vector2f pos, float rot)
{
	auto ball = std::shared_ptr<Entity>(new Entity(g));
	ball->type = EntityType::CannonBall;
	ball->tag = EntityTag::Cannonball;
	ball->components.renderer = std::shared_ptr<RenderComponent>(new SpriteRenderer(
		ball,
		"ships",
		"cannonBall.png",
		RenderManager::INDEX_CANNONBALLS));
	// Set up Box2d definition
	b2BodyDef ballDef;
	ballDef.type = b2_dynamicBody;
	ballDef.gravityScale = 0.0f;
	ballDef.position.Set(pos.x / Game::METER_TO_PIXEL, pos.y / Game::METER_TO_PIXEL);
	b2FixtureDef ballFixture;
	b2CircleShape ballShape;
	// Set up shape for the hitbox
	ballShape.m_radius = 5 / Game::METER_TO_PIXEL;
	ballShape.m_p.Set(-5.0f / Game::METER_TO_PIXEL, -5.0f / Game::METER_TO_PIXEL);
	ballFixture.shape = &ballShape;
	// Make sensor so that the ball creates an explosion when it hits something
	ballFixture.isSensor = true;
	// Set ball team to it's creator's team
	ball->team = spawner.lock()->team;
	ball->components.transform = std::shared_ptr<Box2dTransform>(new Box2dTransform(ball, &ballDef, { ballFixture }, false));
	ball->components.physics = std::shared_ptr<PhysicsComponent>(new PhysicsComponent(ball));
	ball->components.controller = std::shared_ptr<ControllerComponent>(new CannonBallController(ball, rot));
	return ball;
}

std::shared_ptr<Entity> EntityPrefabs::enemyChasingShip(Game* g, sf::Vector2f pos)
{
	auto ship = EntityPrefabs::ship(g, pos, 0.0f, ShipRenderer::SAIL_COLOR::Black);
	ship->type = EntityType::PirateShip;
	ship->team = 1;
	ship->components.controller = std::shared_ptr<ControllerComponent>(new ChasingShipController(ship));
	return ship;
}

std::shared_ptr<Entity> EntityPrefabs::ship(Game* g, sf::Vector2f pos, float rot, ShipRenderer::SAIL_COLOR c)
{
	// Create a ship definition for the new ship
	b2BodyDef shipDef;
	shipDef.type = b2_dynamicBody;
	shipDef.position.Set(pos.x / Game::METER_TO_PIXEL, pos.y / Game::METER_TO_PIXEL);
	shipDef.angle = rot;
	// Create a new fixture and add it to the definition
	b2FixtureDef shipFixture;
	// Create shape for the fixture to have
	b2PolygonShape shipShape; // lol
	// Set the shape to a ship-like pentagon
	b2Vec2 verts[5];
	verts[0].Set(-50 / Game::METER_TO_PIXEL, -20 / Game::METER_TO_PIXEL);
	verts[1].Set(30 / Game::METER_TO_PIXEL, -20 / Game::METER_TO_PIXEL);
	verts[2].Set(50 / Game::METER_TO_PIXEL, 0);
	verts[3].Set(30 / Game::METER_TO_PIXEL, 20 / Game::METER_TO_PIXEL);
	verts[4].Set(-50 / Game::METER_TO_PIXEL, 20 / Game::METER_TO_PIXEL);
	shipShape.Set(verts, 5);
	shipFixture.shape = &shipShape;
	// PhysicsComponent will calculate the volume/mass itself, so this value doesn't really matter
	shipFixture.density = 100;
	// Create the actualy ship entity
	auto ship = std::shared_ptr<Entity>(new Entity(g));
	ship->tag = EntityTag::Ship;
	ship->components.transform = std::shared_ptr<TransformComponent>(new Box2dTransform(ship, &shipDef, { shipFixture }));
	ship->components.renderer = std::shared_ptr<RenderComponent>(new ShipRenderer(ship, c));
	ship->components.physics = std::shared_ptr<PhysicsComponent>(new PhysicsComponent(ship));
	ship->components.health = std::shared_ptr<HealthComponent>(new HealthComponent(ship, 100));
	ship->components.inventory = std::shared_ptr<InventoryComponent>(new InventoryComponent(ship));
	ship->components.gui = std::shared_ptr<GuiComponent>(new GuiComponent(ship));
	ship->components.click = std::shared_ptr<ClickComponent>(new Box2dClick(ship));
	return ship;
}
std::shared_ptr<Entity> EntityPrefabs::militaryBase(Game* g, sf::Vector2i pos)
{
	auto b = base(g, pos);
	b->type = EntityType::MilitaryBase;
	b->components.controller = std::shared_ptr<ControllerComponent>(new MilitaryBaseController(b));
	b->components.renderer = std::shared_ptr<RenderComponent>(new MilitaryBaseRenderer(b));
	// TODO: Make team a parameter
	b->team = 1;
	return b;
}
std::shared_ptr<Entity> EntityPrefabs::miningBase(Game* g, sf::Vector2i pos) {
	auto b = base(g, pos);
	b->type = EntityType::MiningBase;
	b->components.controller = std::shared_ptr<ControllerComponent>(new MiningBaseController(b, GameResource::Stone));
	b->components.renderer = std::shared_ptr<RenderComponent>(new MiningBaseRenderer(b, GameResource::Stone));
	b->team = 1;
	return b;
}
std::shared_ptr<Entity> EntityPrefabs::base(Game* g, sf::Vector2i pos)
{
	b2BodyDef baseDef;
	baseDef.type = b2_staticBody;
	baseDef.position = b2Vec2(pos.x / Game::METER_TO_PIXEL, pos.y / Game::METER_TO_PIXEL);
	baseDef.angle = 0.0f;
	b2FixtureDef baseFixture;
	b2PolygonShape baseShape;
	b2Vec2 verts[4];
	verts[0] = b2Vec2(0, 0);
	verts[1] = b2Vec2(0, 3 * 64 / Game::METER_TO_PIXEL);
	verts[2] = b2Vec2(3 * 64 / Game::METER_TO_PIXEL, 3 * 64 / Game::METER_TO_PIXEL);
	verts[3] = b2Vec2(3 * 64 / Game::METER_TO_PIXEL, 0);
	baseShape.Set(verts, 4);
	baseFixture.shape = &baseShape;

	auto base = std::shared_ptr<Entity>(new Entity(g));
	base->tag = EntityTag::Base;
	base->components.transform = std::shared_ptr<TransformComponent>(new Box2dTransform(base, &baseDef, { baseFixture }));
	base->components.renderer = std::shared_ptr<RenderComponent>(new BaseRenderer(base));
	base->components.gui = std::shared_ptr<GuiComponent>(new GuiComponent(base));
	base->components.click = std::shared_ptr<ClickComponent>(new Box2dClick(base));
	base->components.inventory = std::shared_ptr<InventoryComponent>(new InventoryComponent(base));
	return base;
}
std::shared_ptr<Entity> EntityPrefabs::explosion(Game* g, sf::Vector2f pos)
{
	auto ex = std::shared_ptr<Entity>(new Entity(g));
	ex->type = EntityType::Explosion;
	ex->components.transform = std::shared_ptr<TransformComponent>(new BasicTransform(ex, pos, 0.0f));
	ex->components.renderer = std::shared_ptr<RenderComponent>(new AnimationRenderer(
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
	ship->type = EntityType::Ferry;
	ship->components.controller = std::shared_ptr<ControllerComponent>(new FerryShipController(ship));
	ship->components.gui = std::shared_ptr<GuiComponent>(new GuiComponent(ship));
	return ship;
}
std::shared_ptr<Entity> EntityPrefabs::city(Game* g, sf::Vector2i pos) {
	auto city = EntityPrefabs::base(g, pos);
	city->type = EntityType::City;
	city->components.controller = std::shared_ptr<ControllerComponent>(new CityController(city));
	city->components.renderer = std::shared_ptr<RenderComponent>(new CityRenderer(city));
	return city;
}
std::shared_ptr<Entity> EntityPrefabs::forestryBase(Game* g, sf::Vector2i pos) {
	auto base = EntityPrefabs::base(g, pos);
	base->type = EntityType::ForestryBase;
	base->components.renderer = std::shared_ptr<RenderComponent>(new MiningBaseRenderer(base, GameResource::Wood));
	base->components.controller = std::shared_ptr<ControllerComponent>(new MiningBaseController(base, GameResource::Wood));
	return base;
}
std::shared_ptr<Entity> EntityPrefabs::pirateBase(Game* g, sf::Vector2i pos) {
	auto base = EntityPrefabs::base(g, pos);
	base->type = EntityType::PirateBase;
	base->components.renderer = std::shared_ptr<RenderComponent>(new PirateBaseRenderer(base));
	base->components.controller = std::shared_ptr<ControllerComponent>(new PirateBaseController(base));
	return base;
}
std::shared_ptr<Entity> EntityPrefabs::conversionBase(Game* g, sf::Vector2i pos, GameResource res) {
	auto base = EntityPrefabs::base(g, pos);
	base->type = EntityType::ConversionBase;
	base->components.renderer = std::shared_ptr<RenderComponent>(new MiningBaseRenderer(base, res));
	base->components.controller = std::shared_ptr<ControllerComponent>(new ConversionBaseController(base, res));
	return base;
}

std::shared_ptr<Entity> EntityPrefabs::getEntityFromSaveData(Game* g, std::map<std::string, std::string> data) {
	EntityType type = (EntityType)std::stoi(data["type"]);
	float x = std::stoi(data["x"]);
	float y = std::stoi(data["y"]);
	switch (type){
	// Types of bases
	case EntityType::MiningBase:
		return EntityPrefabs::miningBase(g, { (int)x, (int)y });
	case EntityType::ForestryBase:
		return EntityPrefabs::forestryBase(g, { (int)x, (int)y });
	case EntityType::City:
		return EntityPrefabs::city(g, { (int)x, (int)y });
	case EntityType::MilitaryBase:
		return EntityPrefabs::militaryBase(g, { (int)x, (int)y });
	case EntityType::PirateBase:
		return EntityPrefabs::pirateBase(g, { (int)x, (int)y });
	// Types of ships
	case EntityType::Player:
		return EntityPrefabs::playerShip(g, { x, y });
	case EntityType::PirateShip:
		return EntityPrefabs::enemyChasingShip(g, { x, y });
	case EntityType::Ferry:
		// Source/destination is set in Component::fromSvaeData after all entities are loaded
		return EntityPrefabs::ferryShip(g, { x, y }, std::weak_ptr<Entity>(), std::weak_ptr<Entity>());
	case EntityType::ConversionBase:
		return EntityPrefabs::conversionBase(g, { (int)x, (int)y }, (GameResource)(std::stoi(data["product"])));
	}
	auto breakpoint = 0;
	return nullptr;
}