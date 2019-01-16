#include "PhysicsComponent.h"
#include "Box2dTransform.h"

void PhysicsComponent::setParent(std::weak_ptr<Entity> parent) {
	Component::setParent(parent);
	// Check the transform is a box2d transform
	if (!std::dynamic_pointer_cast<Box2dTransform>(this->getParent().lock()->components.transform)) {
		throw std::runtime_error("An entity has a physics component without a box2d transform, ya dummy");
	}
	this->body = std::dynamic_pointer_cast<Box2dTransform>(this->getParent().lock()->components.transform)->getBody();
	this->world = this->getGame()->getWorld();
}
void PhysicsComponent::setAcceleration(sf::Vector2f a)
{
	this->body->ApplyForceToCenter({ a.x * this->body->GetMass(), a.y * this->body->GetMass() }, true);
}
void PhysicsComponent::setAngularAcceleration(float alpha)
{
	this->body->ApplyTorque(alpha * this->body->GetInertia(), true);
}
void PhysicsComponent::setVelocity(sf::Vector2f v)
{
	this->body->SetLinearVelocity({ v.x * this->body->GetMass(), v.y * this->body->GetMass() });
}
void PhysicsComponent::setNetForce(sf::Vector2f force)
{
	//tba
}