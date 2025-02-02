#pragma once
#include "Component.h"
// Used for getting cos/sin/etc
#define _USE_MATH_DEFINES
#include <math.h>

// Component which holds position and rotation
class TransformComponent : public Component
{
public:
	TransformComponent();
	
	virtual sf::Vector2f getPosition() = 0;
	virtual void setPosition(sf::Vector2f newPos) = 0;

	virtual float getRotation() = 0;
	virtual void setRotation(float newRot) = 0;
	virtual SaveData getSaveData() override;
	virtual void fromSaveData(SaveData data) override;

	// Get difference in position and angle between two transforms
	std::pair<sf::Vector2f, float> getDifference(std::weak_ptr<TransformComponent> other);
	// Get the same difference, but when given a position
	std::pair<sf::Vector2f, float> getDifference(sf::Vector2f other);
};