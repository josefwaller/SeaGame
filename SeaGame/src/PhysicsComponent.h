#pragma once
#include "Component.h"
#include "Box2D\Box2D.h"

// Provides an interface for changing the component's position through physics
// i.e. changing acceleration, rotation, etc
class PhysicsComponent : public Component
{
public:
	virtual void setParent(std::weak_ptr<Entity> parent) override;
	// Set acceleration
	void setAcceleration(sf::Vector2f a);
	void setAngularAcceleration(float alpha);
	// Set velocity
	void setVelocity(sf::Vector2f v);
	// Just apply a force to the body for this frame
	void setNetForce(sf::Vector2f force);
protected:
	// The body that the Bocx2dTransform component uses
	// Will use to control the entity's physics
	b2Body* body;
	// Weak pointer to the b2world
	// Used to destroy the body after the entity has gone out of scope
	std::weak_ptr<b2World> world;
};