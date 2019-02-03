#pragma once
#include <memory>
#include <map>
#include <string>
#include "ComponentList.h"
#include "EntityType.h"

// Forward declarations
class Game;
class SaveData;

enum class EntityTag {
	Ship,
	Base,
	Cannonball,
	Resource,
	Effect
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
	SaveData getSaveData();
	// Get a string representation of the entity to be used in UI
	std::string getStringRep();
};

#include "Game.h"