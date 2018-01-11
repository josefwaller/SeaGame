#pragma once
#include <memory>

// Forward declaration of RenderComponent
class RenderComponent;

struct Entity
{
	std::shared_ptr<RenderComponent> renderer;
};

#include "RenderComponent.h"