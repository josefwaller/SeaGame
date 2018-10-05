#pragma once
#include <memory>
#include <map>
#include <string>
#include "ComponentList.h"

// Forward declaration of Game
class Game;

enum EntityTag {
	Ship,
	Base,
	Cannonball
};

// CHANGING THESE VALUES WILL CAUSE SAVE FILES TO BE BROKEN AF
enum EntityType {
	Player = 0,
	CannonBall = 1,
	PirateShip = 2,
	Explosion = 3,
	MilitaryBase = 4,
	MiningBase = 5,
	ForestryBase = 6,
	Ferry = 7,
	City = 8,
	PirateBase = 9
};

struct Entity
{
	Entity() : game(nullptr) {}
	Entity(Game* g) : game(g) {
		Entity::trueEntityCount++;
	}
	~Entity() {
		Entity::trueEntityCount--;
	}
	Game* game;
	static unsigned int trueEntityCount;
	// The team this entity belongs to
	// 0 is the player's team
	int team;
	// The entity's tag
	EntityTag tag;
	// The specific type of entity
	EntityType type;
	ComponentList components;
	std::map<std::string, std::string> getSaveData();
};

#include "Game.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "ControllerComponent.h"
#include "PhysicsComponent.h"
#include "InventoryComponent.h"
#include "GuiComponent.h"
#include "ClickComponent.h"


