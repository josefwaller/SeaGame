#include "HealthComponent.h"

HealthComponent::HealthComponent(std::weak_ptr<Entity> parent) : Component(parent)
{
}
void HealthComponent::addHealth(HealthType t, int health, int maxHealth) {
	// Optional, can only set health and maxHealth will jsut be set to the same
	if (maxHealth = -1)
		maxHealth = health;
	// Add new type of health
	this->healths.insert({
		t,
		health
	});
}
// Get certain type of health
int HealthComponent::getHealth(HealthType t) {
	if (this->healths.find(t) != this->healths.end())
		return this->healths[t];
	return -1;
}
// Apply damage to a type of health
void HealthComponent::takeDamage(HealthType t, int damageAmount) {
	if (this->healths.find(t) != this->healths.end())
		this->healths[t] = std::max(0, this->healths[t] - damageAmount);
}
// Heal a type of health
void HealthComponent::heal(HealthType t, int healAmount) {
	if (this->healths.find(t) != this->healths.end())
		this->healths[t] = std::min(this->maxHealths[t], this->healths[t] + healAmount);
}
// Check if all healths have been depleted
bool HealthComponent::isDead() {
	for (auto it : this->healths) {
		if (it.second != 0)
			return false;
	}
	return true;
}