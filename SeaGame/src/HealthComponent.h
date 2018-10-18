#pragma once
#include "Component.h"
#include "HealthType.h"

class HealthComponent : public Component
{
public:

	HealthComponent(std::weak_ptr<Entity> parent, unsigned int maxHealth);
	// Get the health this entity has
	virtual int getHealth();
	// Deal damage to this entity
	virtual void takeDamage(int damageAmount);
	// Heal this entity
	virtual void heal(int healAmount);
	// Check if the entity is dead
	virtual bool isDead();
private:
	// The amount of health
	unsigned int health;
	// The max amount of health
	unsigned int maxHealth;
};
