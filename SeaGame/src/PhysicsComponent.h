#pragma once
#include "Component.h"
#include "Box2D\Box2D.h"

// Provides an interface for changing the component's position through physics
// i.e. changing acceleration, rotation, etc
class PhysicsComponent : public Component
{
public:
	PhysicsComponent(std::weak_ptr<Entity> parent);
	// Set acceleration
	void setAcceleration(sf::Vector2f a);
	void setAngularAcceleration(float alpha);
	// Just apply a force to the body for this frame
	void setNetForce(sf::Vector2f force);

protected:
	// The body that the Bocx2dTransform component uses
	// Will use to control the entity's physics
	b2Body* body;
};