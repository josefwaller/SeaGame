#pragma once
#include <SFML\Graphics.hpp>
#include "Component.h"
// Included here bc they are used in a lot of rendering components
// to convert between degrees and radians
#define _USE_MATH_DEFINES
#include <math.h>

class RenderComponent: public Component
{
public:
	RenderComponent(Entity& parent);
	virtual void render(sf::RenderWindow& w) = 0;
};