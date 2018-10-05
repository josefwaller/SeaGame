#pragma once
#include <memory>
#include <map>
#include <string>

// Forward declaration of Game
class Game;

// Forward declaration of components
class RenderComponent;
class TransformComponent;
class HealthComponent;
class ControllerComponent;
class ColliderComponent;
class PhysicsComponent;
class InventoryComponent;
class GuiComponent;
class ClickComponent;

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
	std::shared_ptr<TransformComponent> transform;
	std::shared_ptr<RenderComponent> renderer;
	std::shared_ptr<PhysicsComponent> physics;
	std::shared_ptr<ControllerComponent> controller;
	std::shared_ptr<HealthComponent> health;
	std::shared_ptr<InventoryComponent> inventory;
	std::shared_ptr<GuiComponent> gui;
	std::shared_ptr<ClickComponent> click;
	std::map<std::string, std::string> getSaveData();
};

#include "RenderComponent.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "ControllerComponent.h"
#include "InventoryComponent.h"
#include "GuiComponent.h"
#include "ClickComponent.h"
#include "Game.h"