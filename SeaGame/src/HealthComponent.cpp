#include "HealthComponent.h"
#include "ControllerComponent.h"
#include "SaveData.h"

HealthComponent::HealthComponent(unsigned int maxHealth) : Component()
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
	if (this->isDead()) {
		this->callOnDeath = true;
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
void HealthComponent::update() {
	if (this->callOnDeath) {
		if (this->getComponentList().controller) {
			this->getComponentList().controller->onDeath();
		}
		this->callOnDeath = false;
	}
}
SaveData HealthComponent::getSaveData() {
	return SaveData("Component", {
		{ "health", std::to_string(this->health) }
	});
}
void HealthComponent::fromSaveData(SaveData data) {
	this->health = std::stoi(data.getValue("health"));
}