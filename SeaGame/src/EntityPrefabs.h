#pragma once
#include <memory>
#include "Game.h"
#include "Entity.h"
#include "ShipRenderer.h"

using entity_ptr = std::shared_ptr<Entity>;
// Contain methods to easily create different types of entities
class EntityPrefabs
{
public:
	// Called for every entity before being created in the game
	// Anything to initialize each entity (ex: wrap in shared_ptr) should be done in here
	static entity_ptr buildEntity(Entity* entity);
	// Return the different type of entities in the game
	static entity_ptr playerShip(Game* g, sf::Vector2f position);
	static entity_ptr cannonBall(Game* g, std::weak_ptr<Entity> spawner, sf::Vector2f pos, float rot);
	static entity_ptr enemyChasingShip(Game* g, sf::Vector2f pos);
	static entity_ptr explosion(Game* g, sf::Vector2f pos);
	static entity_ptr militaryBase(Game* g, sf::Vector2i pos);
	static entity_ptr generationBase(Game* g, sf::Vector2i pos, GameResource res);
	static entity_ptr ferryShip(Game * g, sf::Vector2f pos, std::weak_ptr<Entity> from, std::weak_ptr<Entity> to);
	static entity_ptr city(Game* g, sf::Vector2i pos);
	static entity_ptr pirateBase(Game* g, sf::Vector2i pos);
	static entity_ptr conversionBase(Game* g, sf::Vector2i pos, GameResource res);
	static entity_ptr resourceSource(Game* g, sf::Vector2i pos, GameResource res);
	// Get the Box2d BodyDef and fixtures needed for specific entities
	static b2Body* getShipBody(Game* g, sf::Vector2f pos, float rot);
	static b2Body* getCannonballBody(Game* g, sf::Vector2f pos, float rot);
	static b2Body* getBaseBody(Game* g, sf::Vector2f pos, float rot);
	// Get an entity from its save data
	static entity_ptr getEntityFromSaveData(Game* g, std::map<std::string, std::string> data);
private:
	// Map of EntityTypes -> Unique string representation of the type
	std::map<EntityType, std::string> typeStrings;
};