#pragma once
#include "Component.h"
#include "HealthType.h"

class HealthComponent : public Component
{
public:

	HealthComponent(std::weak_ptr<Entity> parent);
	// Add a type of health to this entity
	void addHealth(HealthType t, int health, int maxHealth = -1);
	// Get the health this entity has
	virtual int getHealth(HealthType t);
	// Deal damage to this entity
	virtual void takeDamage(HealthType t, int damageAmount);
	// Heal this entity
	virtual void heal(HealthType t, int healAmount);
	// Check if the entity is dead
	virtual bool isDead();
private:
	std::map<HealthType, int> healths;
	std::map<HealthType, int> maxHealths;
};
