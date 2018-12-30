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
	// Check if the entity is dead
	virtual bool isDead();
	void updateGui(tgui::Tabs::Ptr, std::map<std::string, tgui::Panel::Ptr>* panels) override;
	virtual SaveData getSaveData() override;
	virtual void fromSaveData(SaveData data) override;
private:
	// The amount of health
	unsigned int health;
	// The max amount of health
	unsigned int maxHealth;
};
