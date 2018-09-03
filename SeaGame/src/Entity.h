#pragma once
#include <memory>

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

struct Entity
{
	Entity() : game(nullptr) {}
	Entity(Game* g) : game(g) {}
	Game* game;
	// The team this entity belongs to
	// 0 is the player's team
	int team;
	std::shared_ptr<TransformComponent> transform;
	std::shared_ptr<RenderComponent> renderer;
	std::shared_ptr<PhysicsComponent> physics;
	std::shared_ptr<ControllerComponent> controller;
	std::shared_ptr<HealthComponent> health;
	std::shared_ptr<InventoryComponent> inventory;
};

#include "RenderComponent.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "ControllerComponent.h"
#include"InventoryComponent.h"
#include "Game.h"