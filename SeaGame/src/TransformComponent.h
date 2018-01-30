#pragma once
#include "Component.h"

// Component which holds position and rotation
// Note: the only Transform component has both position and rotation
// Each entity can either have both or none
class TransformComponent : public Component
{
public:
	TransformComponent(std::weak_ptr<Entity> parent, sf::Vector2f initPos, float initRot);
	
	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f newPos);

	float getRotation();
	void setRotation(float newRot);
	// Get difference in position and angle between two transforms
	std::pair<sf::Vector2f, float> getDifference(TransformComponent other);

private:
	sf::Vector2f position;
	float rotation;
};