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
	if (this->getParent().lock()->health != nullptr)
		this->getParent().lock()->health->takeDamage(t, damageAmount);
}
void ControllerComponent::onDeath() {}
void ControllerComponent::onClick() {
	if (this->getParent().lock()->gui) {
		this->getParent().lock()->gui->show();
	}
}