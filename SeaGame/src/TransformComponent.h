#pragma once
#include "Component.h"

// Component which holds position and rotation
class TransformComponent : public Component
{
public:
	TransformComponent(std::weak_ptr<Entity> parent);
	
	virtual sf::Vector2f getPosition() = 0;
	virtual void setPosition(sf::Vector2f newPos) = 0;

	virtual float getRotation() = 0;
	virtual void setRotation(float newRot) = 0;

	// Get difference in position and angle between two transforms
	std::pair<sf::Vector2f, float> getDifference(std::weak_ptr<TransformComponent> other);
};