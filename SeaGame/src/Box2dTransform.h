#pragma once
#include "TransformComponent.h"

class Box2dTransform : public TransformComponent
{
public:
	Box2dTransform(std::weak_ptr<Entity> parent, b2BodyDef* def, std::vector<b2FixtureDef> fixtures);

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
};