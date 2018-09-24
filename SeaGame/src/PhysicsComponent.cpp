#include "PhysicsComponent.h"
#include "Box2dTransform.h"

PhysicsComponent::PhysicsComponent(std::weak_ptr<Entity> parent) : Component(parent)
{
	if (!std::dynamic_pointer_cast<Box2dTransform>(this->getParent().lock()->transform)) {
		throw std::runtime_error("An entity has a physics component without a box2d transform, ya dummy");
	}
	this->body = std::dynamic_pointer_cast<Box2dTransform>(this->getParent().lock()->transform)->getBody();
	// Set the parent as the user data
	this->body->SetUserData(this);
	this->world = this->getParent().lock()->game->getWorld();
}
PhysicsComponent::~PhysicsComponent()
{
	// Destory the body
	this->world.lock()->DestroyBody(this->body);
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
	this->body->SetLinearVelocity({ v.x, v.y });
}
void PhysicsComponent::setNetForce(sf::Vector2f force)
{
	//tba
}