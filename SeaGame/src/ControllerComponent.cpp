#include "ControllerComponent.h"

ControllerComponent::ControllerComponent(std::weak_ptr<Entity> e) : Component(e)
{

}

// Empty by default, can be implemented in child class
void ControllerComponent::onCollision(std::weak_ptr<Entity> other)
{

}