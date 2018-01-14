#pragma once
#include <memory>

// Forward declaration of components
class RenderComponent;
class TransformComponent;

struct Entity
{
	std::shared_ptr<TransformComponent> transform;
	std::shared_ptr<RenderComponent> renderer;
};

#include "RenderComponent.h"
#include "TransformComponent.h"