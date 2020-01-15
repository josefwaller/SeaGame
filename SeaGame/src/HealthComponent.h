#pragma once
#include "Component.h"
#include "HealthType.h"

class HealthComponent : public Component
{
public:

	HealthComponent(unsigned int maxHealth);
	// Get the health this entity has
	virtual int getHealth();
	// Deal damage to this entity
	virtual void takeDamage(int damageAmount);
	// Heal this entity
	virtual void heal(int healAmount);
	// Update, or when it is same to call ControllerComponent::onDeath()
	// So that we don't call it when the box 2d world is stepping
	void update();
	// Check if the entity is dead
	virtual bool isDead();
	virtual SaveData getSaveData() override;
	virtual void fromSaveData(SaveData data) override;
private:
	// The amount of health
	unsigned int health;
	// The max amount of health
	unsigned int maxHealth;
	// Whether to call onDeath on the next update
	bool callOnDeath;
};
