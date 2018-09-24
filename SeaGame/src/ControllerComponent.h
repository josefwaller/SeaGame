#pragma once
#include "Component.h"
#include "HealthType.h"

class ControllerComponent : public Component
{
public:
	ControllerComponent(std::weak_ptr<Entity> e);
	virtual void update(float delta) = 0;
	// Resolve collisions with other entities
	virtual void onCollision(std::weak_ptr<Entity> other);
	// Method when taking damage
	virtual void onHit(HealthType t, int damageAmount);
	virtual void onDeath();
	// When the entity is clicked
	virtual void onClick();
};