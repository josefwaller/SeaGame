#pragma once
#include "TransformComponent.h"

// Component which holds position and rotation
class BasicTransform : public TransformComponent
{
public:
	BasicTransform(sf::Vector2f initPos, float initRot);

	sf::Vector2f getPosition() override;
	void setPosition(sf::Vector2f newPos) override;

	float getRotation() override;
	void setRotation(float newRot) override;

private:
	sf::Vector2f position;
	float rotation;
};