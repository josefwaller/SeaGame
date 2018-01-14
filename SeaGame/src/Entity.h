#pragma once
#include <memory>

// Forward declaration of components
class RenderComponent;
class TransformComponent;
class ControllerComponent;

struct Entity
{
	std::shared_ptr<TransformComponent> transform;
	std::shared_ptr<RenderComponent> renderer;
	std::shared_ptr<ControllerComponent> controller;
};

#include "RenderComponent.h"
#include "TransformComponent.h"
#include "ControllerComponent.h"