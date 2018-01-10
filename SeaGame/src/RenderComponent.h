#pragma once
#include <SFML\Graphics.hpp>

// Forward declaration of Entity
struct Entity;

class RenderComponent
{
public:
	RenderComponent(Entity& parent);
	virtual void render(sf::RenderWindow& w) = 0;

protected:
	Entity& parent;
};

#include "Entity.h"