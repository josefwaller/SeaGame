#include "ControllerComponent.h"
#include "GuiComponent.h"

ControllerComponent::ControllerComponent() : Component()
{

}

// Empty by default, can be implemented in child class
void ControllerComponent::onCollision(std::weak_ptr<Entity> other)
{

}

void ControllerComponent::onHit(HealthType t, int damageAmount)
{
	// Default, just reduces overall health
	if (this->getComponentList().health != nullptr)
		this->getComponentList().health->takeDamage(damageAmount);
}
void ControllerComponent::onDeath() {}
void ControllerComponent::onClick() {
	if (this->getComponentList().gui) {
		this->getGame()->getHud()->showEntity(this->getParent());
	}
}