#include "HealthComponent.h"

HealthComponent::HealthComponent(std::weak_ptr<Entity> parent, unsigned int maxHealth) : Component(parent)
{
	this->maxHealth = maxHealth;
	this->health = maxHealth;
}
// Get certain type of health
int HealthComponent::getHealth() {
	return this->health;
	return -1;
}
// Apply damage to a type of health
void HealthComponent::takeDamage(int damageAmount) {
	this->health = std::max(0, (int)this->health - damageAmount);
	if (this->isDead() && this->getParent().lock()->components.controller) {
		this->getParent().lock()->components.controller->onDeath();
	}
}
// Heal a type of health
void HealthComponent::heal(int healAmount) {
	this->health = std::min(this->maxHealth, this->health + healAmount);
}
// Check if all healths have been depleted
bool HealthComponent::isDead() {
	return this->health == 0;
}