#pragma once
#include <memory>
#include <map>
#include <string>
#include "ComponentList.h"

// Forward declaration of Game
class Game;

enum class EntityTag {
	Ship,
	Base,
	Cannonball,
	Resource,
	Effect
};

// CHANGING THESE VALUES WILL CAUSE SAVE FILES TO BE BROKEN AF
enum class EntityType {
	Player = 0,
	Cannonball = 1,
	PirateShip = 2,
	Explosion = 3,
	MilitaryBase = 4,
	MiningBase = 5,
	ForestryBase = 6,
	Ferry = 7,
	City = 8,
	PirateBase = 9,
	ConversionBase = 10,
	ResourceDeposit = 11
};
struct Entity
{
	Entity() : game(nullptr) {}
	Entity(Game* g, int team, EntityType type, EntityTag tag, ComponentList c)
		: game(g), team(team), type(type), tag(tag), components(c) {
		// Set id
		Entity::trueEntityCount++;
		this->id = Entity::currentId;
		Entity::currentId++;
	}
	~Entity() {
		Entity::trueEntityCount--;
	}
	Game* game;
	static unsigned int trueEntityCount;
	static unsigned long currentId;
	// The team this entity belongs to
	// 0 is the player's team
	int team;
	// The entity's id
	unsigned long id;
	// The entity's tag
	EntityTag tag;
	// The specific type of entity
	EntityType type;
	ComponentList components;
	std::map<std::string, std::string> getSaveData();
	// Get a string representation of the entity to be used in UI
	std::string getStringRep();
};

#include "Game.h"