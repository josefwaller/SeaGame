#pragma once
#include "TransformComponent.h"

class Box2dTransform : public TransformComponent
{
public:
	// Damping to slow down moving and turning
	static const float LINEAR_DAMPING;
	static const float ANGULAR_DAMPING;

	Box2dTransform(b2Body* body);
	~Box2dTransform() override;
	virtual void setParent(std::weak_ptr<Entity> parent) override;

	sf::Vector2f getPosition() override;
	virtual void setPosition(sf::Vector2f newPos) override;

	virtual float getRotation() override;
	virtual void setRotation(float newAngle) override;
	// Get the box2d body
	// Used by PhysicsComponent, which will then hold a weak_ptr for reference
	b2Body* getBody();
private:
	// The body the entity is attached to
	b2Body* body;
	// World the body is in, to easily dstroy it
	std::weak_ptr<b2World> world;
};