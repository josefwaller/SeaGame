#include "ControllerComponent.h"

ControllerComponent::ControllerComponent(std::weak_ptr<Entity> e) : Component(e)
{

}

// Empty by default, can be implemented in child class
void ControllerComponent::onCollision(std::weak_ptr<Entity> other)
{

}

void ControllerComponent::onHit(HealthType t, int damageAmount)
{
	// Default, just reduces overall health
	if (this->getParent()->health != nullptr)
		this->getParent()->health->takeDamage(t, damageAmount);
}
void ControllerComponent::onClick() {
	if (this->getParent()->gui) {
		this->getParent()->gui->show();
	}
}