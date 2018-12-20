#pragma once
#include <memory>
#include "Game.h"
#include "Entity.h"
#include "ShipRenderer.h"

// Contain methods to easily create different types of entities
class EntityPrefabs
{
public:
	// Called for every entity before being created in the game
	// Anything to initialize each entity (ex: wrap in shared_ptr) should be done in here
	static std::shared_ptr<Entity> buildEntity(Entity* entity);
	// Return the different type of entities in the game
	static std::shared_ptr<Entity> playerShip(Game* g, sf::Vector2f position);
	static std::shared_ptr<Entity> cannonBall(Game* g, std::weak_ptr<Entity> spawner, sf::Vector2f pos, float rot);
	static std::shared_ptr<Entity> enemyChasingShip(Game* g, sf::Vector2f pos);
	static std::shared_ptr<Entity> explosion(Game* g, sf::Vector2f pos);
	static std::shared_ptr<Entity> militaryBase(Game* g, sf::Vector2i pos);
	static std::shared_ptr<Entity> generationBase(Game* g, sf::Vector2i pos, GameResource res);
	static std::shared_ptr<Entity> ferryShip(Game * g, sf::Vector2f pos, std::weak_ptr<Entity> from, std::weak_ptr<Entity> to);
	static std::shared_ptr<Entity> city(Game* g, sf::Vector2i pos);
	static std::shared_ptr<Entity> pirateBase(Game* g, sf::Vector2i pos);
	static std::shared_ptr<Entity> getEntityFromSaveData(Game* g, std::map<std::string, std::string> data);
	static std::shared_ptr<Entity> conversionBase(Game* g, sf::Vector2i pos, GameResource res);
	static std::shared_ptr<Entity> resourceSource(Game* g, sf::Vector2i pos, GameResource res);
	// Get the Box2d BodyDef and fixtures needed for specific entities
	static b2Body* getShipBody(Game* g, sf::Vector2f pos, float rot);
	static b2Body* getCannonBallBody(Game* g, sf::Vector2f pos, float rot);
	static b2Body* getBaseBody(Game* g, sf::Vector2f pos, float rot);
	// Get the EntityType from a string representation
	static EntityType getTypeFromString(std::string typeStr);
	// Get a string representation from an EntityType
	static std::string getStringFromEntityType(EntityType t);
private:
	// Map of EntityTypes -> Unique string representation of the type
	std::map<EntityType, std::string> typeStrings;
};