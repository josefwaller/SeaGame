#include "EntityPrefabs.h"
#include "SaveData.h"
#include "Game.h"
#include "Entity.h"
#include "Box2dTransform.h"
#include <SFML/System/Vector2.hpp>
#include <Box2D/Box2D.h>

// Wrap entity in shared pointer and set parent/component relationship
entity_ptr EntityPrefabs::buildEntity(Entity* entity) {
	// Wrap in shared pointer
	entity_ptr e = entity_ptr(entity);
	// Set the parent
	e->components.set(e);
	// Return entity
	return e;
}


entity_ptr EntityPrefabs::getEntityFromSaveData(Game* g, SaveData data) {
	EntityType type = (EntityType)std::stoi(data.getValue("type"));
	float x = 0.0f;
	float y = 0.0f;
	return getEntityFromType(g, { x, y }, type);
	return nullptr;
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
	baseFixture.density = 100;
	b2Body* b = g->getWorld().lock()->CreateBody(&baseDef);
	b->CreateFixture(&baseFixture);
	return b;
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

entity_ptr EntityPrefabs::getEntityFromType(Game* g, sf::Vector2f pos, EntityType type) {
	float x = pos.x;
	float y = pos.y;
	auto t = type;
	switch (type){
	// Types of bases
	case EntityType::MiningBase:
		return EntityPrefabs::generationBase(g, { x, y }, GameResource::Stone);
	case EntityType::ForestryBase:
		return EntityPrefabs::generationBase(g, { x, y }, GameResource::Wood);
	case EntityType::City:
		return EntityPrefabs::city(g, { x, y });
	case EntityType::MilitaryBase:
		return EntityPrefabs::militaryBase(g, { x, y });
	case EntityType::PirateBase:
		return EntityPrefabs::pirateBase(g, { x, y });
	// Types of ships
	case EntityType::Player:
		return EntityPrefabs::playerShip(g, { x, y });
	case EntityType::PirateShip:
		return EntityPrefabs::enemyChasingShip(g, { x, y });
	case EntityType::Ferry:
		// Source/destination is set in Component::fromSvaeData after all entities are loaded
		return EntityPrefabs::ferryShip(g, { x, y }, std::weak_ptr<Entity>(), std::weak_ptr<Entity>());
	case EntityType::ConversionBase:
		return EntityPrefabs::conversionBase(g, { x, y }, GameResource::Stone);
	case EntityType::ResourceDeposit:
		return EntityPrefabs::resourceSource(g, { x, y }, GameResource::Stone);
	case EntityType::DefensePirateShip:
		return EntityPrefabs::defensePirateShip(g, { x, y }, {});
	case EntityType::Cannonball:
		return EntityPrefabs::cannonBall(g, 0, { x, y }, 0.0f);
	case EntityType::PirateFortress:
		return EntityPrefabs::pirateFortress(g, { x, y });
	case EntityType::AttackPirateShip:
		return EntityPrefabs::attackPirateShip(g, { x, y }, {}, {});
	case EntityType::Explosion:
		return EntityPrefabs::explosion(g, { x, y });
	}
	auto breakpoint = 0;
}