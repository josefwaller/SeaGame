#pragma once
#include <SFML\Graphics.hpp>
#include "Component.h"
// Included because a lot of renderers get position to render
#include "TransformComponent.h"
#include "RenderManager.h"
// Included here bc they are used in a lot of rendering components
// to convert between degrees and radians
#define _USE_MATH_DEFINES
#include <math.h>

class RenderComponent: public Component
{
public:
	virtual void render(RenderManager& r) = 0;
	void renderCollider(RenderManager& r);
	// Update what to render
	// For ships, may change sprites based on damage, aim swivel, etc
	// Empty by default
	virtual void reset();
};