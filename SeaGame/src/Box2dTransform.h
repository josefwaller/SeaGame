#pragma once
#include "TransformComponent.h"

class Box2dTransform : public TransformComponent
{
public:
	// Damping to slow down moving and turning
	static const float LINEAR_DAMPING;
	static const float ANGULAR_DAMPING;

	Box2dTransform(std::weak_ptr<Entity> parent, b2BodyDef* def, std::vector<b2FixtureDef> fixtures, bool addDamp = true);

	sf::Vector2f getPosition() override;
	virtual void setPosition(sf::Vector2f newPos) override;

	virtual float getRotation() override;
	virtual void setRotation(float newAngle) override;
	std::map<std::string, std::string> getSaveData() override;
	// Get the box2d body
	// Used by PhysicsComponent, which will then hold a weak_ptr for reference
	b2Body* getBody();
private:
	// The body the entity is attached to
	b2Body* body;
};