#pragma once
#include <memory>
#include "RenderComponent.h"

struct Entity
{
	std::shared_ptr<RenderComponent> renderer;
};