#pragma once
#include <SFML\Graphics.hpp>
#include "Component.h"

class RenderComponent: public Component
{
public:
	RenderComponent(Entity& parent);
	virtual void render(sf::RenderWindow& w) = 0;
};