#pragma once
#include <memory>

// Forward declaration of Game
class Game;

// Forward declaration of components
class RenderComponent;
class TransformComponent;
class ControllerComponent;

struct Entity
{
	Entity() : game(nullptr) {}
	Entity(Game* g) : game(g) {}
	Game* game;
	std::shared_ptr<TransformComponent> transform;
	std::shared_ptr<RenderComponent> renderer;
	std::shared_ptr<ControllerComponent> controller;
};

#include "RenderComponent.h"
#include "TransformComponent.h"
#include "ControllerComponent.h"
#include "Game.h"