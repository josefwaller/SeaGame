#pragma once

// Forward declaration of Entity
struct Entity;

class Component
{
public:
	Component(Entity& parent);
	virtual Entity& getParent();
private:
	Entity& parent;
};

#include "Entity.h"