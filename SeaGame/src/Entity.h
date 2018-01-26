#pragma once
#include <memory>

// Forward declaration of Game
class Game;

// Forward declaration of components
class RenderComponent;
class TransformComponent;
class ControllerComponent;
class ColliderComponent;

struct Entity
{
	Entity() : game(nullptr) {}
	Entity(Game* g) : game(g) {}
	Game* game;
	std::shared_ptr<TransformComponent> transform;
	std::shared_ptr<RenderComponent> renderer;
	std::shared_ptr<ControllerComponent> controller;
	std::shared_ptr<ColliderComponent> collider;
};

#include "RenderComponent.h"
#include "TransformComponent.h"
#include "ControllerComponent.h"
#include "ColliderComponent.h"
#include "Game.h"