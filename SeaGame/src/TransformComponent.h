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

private:
	sf::Vector2f position;
	float rotation;
};