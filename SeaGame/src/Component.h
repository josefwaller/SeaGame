#pragma once
#include <memory>

// Forward declaration of Entity
struct Entity;

class Component
{
public:
	Component(std::weak_ptr<Entity> parent);
	virtual std::shared_ptr<Entity> getParent();
private:
	std::weak_ptr<Entity> parent;
};

#include "Entity.h"