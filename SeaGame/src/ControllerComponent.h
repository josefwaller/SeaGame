#pragma once
#include "Component.h"

class ControllerComponent : public Component
{
public:
	ControllerComponent(std::weak_ptr<Entity> e);
	virtual void update(float delta) = 0;
	// Resolve collisions with other entities
	virtual void onCollision(std::weak_ptr<Entity> other);
};