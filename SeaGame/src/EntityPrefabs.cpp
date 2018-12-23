#include "EntityPrefabs.h"
#include "ShipRenderer.h"
#include "SpriteRenderer.h"
#include "PlayerShipController.h"
#include "ChasingShipController.h"
#include "CannonballController.h"
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
#include "ResourceController.h"
#include "ResourceRenderer.h"
#include "RectangleClick.h"

// Wrap entity in shared pointer and set parent/component relationship
std::shared_ptr<Entity> EntityPrefabs::buildEntity(Entity* entity) {
	// Wrap in shared pointer
	std::shared_ptr<Entity> e = std::shared_ptr<Entity>(entity);
	// Set the parent
	e->components.set(e);
	// Return entity
	return e;
}

std::shared_ptr<Entity> EntityPrefabs::playerShip(Game* g, sf::Vector2f position)
{
	return EntityPrefabs::buildEntity(new Entity(
		g,
		0,
		EntityType::Player,
		EntityTag::Ship,
		ComponentList(
			new Box2dTransform(EntityPrefabs::getShipBody(g, position, 0.0f)),
			new PlayerShipController(),
			new ShipRenderer(ShipRenderer::SailColor::Blue),
			new PhysicsComponent(),
			new HealthComponent(100),
			new InventoryComponent(),
			new GuiComponent(),
			new Box2dClick()
		)
	));
}
// Create a new ship body
b2Body* EntityPrefabs::getShipBody(Game* g, sf::Vector2f pos, float rot) {
	// Create a ship definition for the new ship
	b2BodyDef shipDef;
	shipDef.type = b2_dynamicBody;
	shipDef.position.Set(pos.x * Game::WORLD_TO_BOX2D, pos.y * Game::WORLD_TO_BOX2D);
	shipDef.angle = rot;
	// Add the fixture
	b2FixtureDef shipFixture;
	// Create shape for the fixture to have
	b2PolygonShape shipShape; // lol
	// Set the shape to a ship-like pentagon
	b2Vec2 verts[5];
	verts[0].Set(-50 * Game::WORLD_TO_BOX2D, -20 * Game::WORLD_TO_BOX2D);
	verts[1].Set(30 * Game::WORLD_TO_BOX2D, -20 * Game::WORLD_TO_BOX2D);
	verts[2].Set(50 * Game::WORLD_TO_BOX2D, 0);
	verts[3].Set(30 * Game::WORLD_TO_BOX2D, 20 * Game::WORLD_TO_BOX2D);
	verts[4].Set(-50 * Game::WORLD_TO_BOX2D, 20 * Game::WORLD_TO_BOX2D);
	shipShape.Set(verts, 5);
	shipFixture.shape = &shipShape;
	// PhysicsComponent will calculate the volume/mass itself, so this value doesn't really matter
	shipFixture.density = 100;
	// Create the body
	b2Body* b = g->getWorld().lock()->CreateBody(&shipDef);
	// Set linear dampning to simulate water
	b->SetLinearDamping(Box2dTransform::LINEAR_DAMPING);
	b->SetAngularDamping(Box2dTransform::ANGULAR_DAMPING);
	// Add fixture
	b->CreateFixture(&shipFixture);
	return b;
}
std::shared_ptr<Entity> EntityPrefabs::cannonBall(Game* g, std::weak_ptr<Entity> spawner, sf::Vector2f pos, float rot)
{
	return buildEntity(new Entity(
		g,
		spawner.lock()->team,
		EntityType::Cannonball,
		EntityTag::Cannonball,
		ComponentList(
			new Box2dTransform(EntityPrefabs::getCannonballBody(g, pos, rot)),
			new CannonballController(rot),
			new SpriteRenderer("ships", "cannonBall.png", RenderManager::INDEX_CANNONBALLS),
			new PhysicsComponent(),
			nullptr,
			nullptr,
			nullptr,
			nullptr
		)
	));
}
b2Body* EntityPrefabs::getCannonballBody(Game* g, sf::Vector2f pos, float rot) {
	// Set up Box2d definition
	b2BodyDef ballDef;
	ballDef.type = b2_dynamicBody;
	ballDef.gravityScale = 0.0f;
	ballDef.position.Set(pos.x * Game::WORLD_TO_BOX2D, pos.y * Game::WORLD_TO_BOX2D);
	b2FixtureDef ballFixture;
	b2CircleShape ballShape;
	// Set up shape for the hitbox
	ballShape.m_radius = 5 * Game::WORLD_TO_BOX2D;
	ballShape.m_p.Set(-5.0f * Game::WORLD_TO_BOX2D, -5.0f * Game::WORLD_TO_BOX2D);
	ballFixture.shape = &ballShape;
	// Make sensor so that the ball creates an explosion when it hits something
	ballFixture.isSensor = true;
	// Create the body
	b2Body* b = g->getWorld().lock()->CreateBody(&ballDef);
	b->CreateFixture(&ballFixture);
	return b;
}
std::shared_ptr<Entity> EntityPrefabs::enemyChasingShip(Game* g, sf::Vector2f pos)
{
	return buildEntity(new Entity(
		g,
		1,
		EntityType::PirateShip,
		EntityTag::Ship,
		ComponentList(
			new Box2dTransform(EntityPrefabs::getShipBody(g, pos, 0.0f)),
			new ChasingShipController(),
			new ShipRenderer(ShipRenderer::SailColor::Black),
			new PhysicsComponent(),
			new HealthComponent(100),
			new InventoryComponent(),
			new GuiComponent(),
			new Box2dClick()
		)
	));
}
std::shared_ptr<Entity> EntityPrefabs::ferryShip(Game* g, sf::Vector2f pos, std::weak_ptr<Entity> from, std::weak_ptr<Entity> to) {
	return EntityPrefabs::buildEntity(new Entity(
		g,
		0,
		EntityType::Ferry,
		EntityTag::Ship,
		ComponentList(
			new Box2dTransform(EntityPrefabs::getShipBody(g, pos, 0.0f)),
			new FerryShipController(),
			new ShipRenderer(ShipRenderer::SailColor::Red),
			new PhysicsComponent(),
			new HealthComponent(100),
			new InventoryComponent(),
			new GuiComponent(),
			new Box2dClick()
		)
	));
}
std::shared_ptr<Entity> EntityPrefabs::militaryBase(Game* g, sf::Vector2i pos)
{
	return buildEntity(new Entity(
		g,
		0,
		EntityType::MilitaryBase,
		EntityTag::Base,
		ComponentList(
			new Box2dTransform(EntityPrefabs::getBaseBody(g, sf::Vector2f(pos), 0.0f)),
			new MilitaryBaseController(),
			new MilitaryBaseRenderer(),
			nullptr,
			new HealthComponent(100),
			new InventoryComponent(),
			new GuiComponent(),
			new Box2dClick()
		)
	));
}
std::shared_ptr<Entity> EntityPrefabs::generationBase(Game* g, sf::Vector2i pos, GameResource res) {
	return EntityPrefabs::buildEntity(new Entity(
		g,
		0,
		// ToDo: Have more specific types for this
		EntityType::MiningBase,
		EntityTag::Base,
		ComponentList(
			new Box2dTransform(EntityPrefabs::getBaseBody(g, sf::Vector2f(pos), 0.0f)),
			new MiningBaseController(res),
			new MiningBaseRenderer(res),
			nullptr,
			new HealthComponent(100),
			new InventoryComponent(),
			new GuiComponent(),
			new Box2dClick()
		)
	));
}
std::shared_ptr<Entity> EntityPrefabs::pirateBase(Game* g, sf::Vector2i pos) {
	return EntityPrefabs::buildEntity(new Entity(
		g,
		1,
		EntityType::PirateBase,
		EntityTag::Base,
		ComponentList(
			new Box2dTransform(EntityPrefabs::getBaseBody(g, sf::Vector2f(pos), 0.0f)),
			new PirateBaseController(),
			new PirateBaseRenderer(),
			nullptr,
			new HealthComponent(100),
			new InventoryComponent(),
			new GuiComponent(),
			new Box2dClick()
		)
	));
}
std::shared_ptr<Entity> EntityPrefabs::conversionBase(Game* g, sf::Vector2i pos, GameResource res) {
	return EntityPrefabs::buildEntity(new Entity(
		g,
		0,
		EntityType::ConversionBase,
		EntityTag::Base,
		ComponentList(
			new Box2dTransform(EntityPrefabs::getBaseBody(g, sf::Vector2f(pos), 0.0f)),
			new ConversionBaseController(res),
			new MiningBaseRenderer(res),
			nullptr,
			new HealthComponent(100),
			new InventoryComponent(),
			new GuiComponent(),
			new Box2dClick()
		)
	));
}
b2Body* EntityPrefabs::getBaseBody(Game* g, sf::Vector2f pos, float rot) {
	b2BodyDef baseDef;
	baseDef.type = b2_staticBody;
	baseDef.position = b2Vec2(pos.x * Game::WORLD_TO_BOX2D, pos.y * Game::WORLD_TO_BOX2D);
	baseDef.angle = 0.0f;
	b2FixtureDef baseFixture;
	b2PolygonShape baseShape;
	b2Vec2 verts[4];
	verts[0] = b2Vec2(0, 0);
	verts[1] = b2Vec2(0, 3 * 64 * Game::WORLD_TO_BOX2D);
	verts[2] = b2Vec2(3 * 64 * Game::WORLD_TO_BOX2D, 3 * 64 * Game::WORLD_TO_BOX2D);
	verts[3] = b2Vec2(3 * 64 * Game::WORLD_TO_BOX2D, 0);
	baseShape.Set(verts, 4);
	baseFixture.shape = &baseShape;
	b2Body* b = g->getWorld().lock()->CreateBody(&baseDef);
	b->CreateFixture(&baseFixture);
	return b;
}
std::shared_ptr<Entity> EntityPrefabs::explosion(Game* g, sf::Vector2f pos)
{
	return EntityPrefabs::buildEntity(new Entity(
		g,
		0,
		EntityType::Explosion,
		EntityTag::Effect,
		ComponentList(
			new BasicTransform(pos, 0.0f),
			nullptr,
			new AnimationRenderer(
				{
					ResourceManager::get()->getSprite("ships", "explosion3.png", true),
					ResourceManager::get()->getSprite("ships", "explosion2.png", true),
					ResourceManager::get()->getSprite("ships", "explosion1.png", true)
				},
				100.0f,
				RenderManager::INDEX_EFFECT,
				false
			),
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr
		)
	));
}
std::shared_ptr<Entity> EntityPrefabs::city(Game* g, sf::Vector2i pos) {
	return buildEntity(new Entity(
		g,
		0,
		EntityType::City,
		EntityTag::Base,
		ComponentList(
			new Box2dTransform(getBaseBody(g, sf::Vector2f(pos), 0.0f)),
			new CityController(),
			new CityRenderer(),
			nullptr,
			nullptr,
			new InventoryComponent(),
			new GuiComponent(),
			new Box2dClick()
		)
	));
}
std::shared_ptr<Entity> EntityPrefabs::resourceSource(Game* g, sf::Vector2i pos, GameResource res) {
	return buildEntity(new Entity(
		g,
		0,
		EntityType::IronVein,
		EntityTag::Resource,
		ComponentList(
			new BasicTransform(sf::Vector2f(pos) * 64.0f, 0.0f),
			new ResourceController(res, 200),
			new ResourceRenderer(res),
			nullptr,
			nullptr,
			new InventoryComponent(),
			new GuiComponent(),
			new RectangleClick(sf::Vector2i(0, 0), sf::Vector2i(64, 64))
		)
	));
}
std::shared_ptr<Entity> EntityPrefabs::getEntityFromSaveData(Game* g, std::map<std::string, std::string> data) {
	EntityType type = (EntityType)std::stoi(data["type"]);
	float x = (float)std::stoi(data["x"]);
	float y = (float)std::stoi(data["y"]);
	switch (type){
	// Types of bases
	case EntityType::MiningBase:
		return EntityPrefabs::generationBase(g, { (int)x, (int)y }, GameResource::Stone);
	case EntityType::ForestryBase:
		return EntityPrefabs::generationBase(g, { (int)x, (int)y }, GameResource::Wood);
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