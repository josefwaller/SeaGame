#include "TransformComponent.h"
#include <iostream>

TransformComponent::TransformComponent(std::weak_ptr<Entity> e, sf::Vector2f initPos = { 0, 0 }, float initRot = 0.0f): Component(e)
{
	this->position = initPos;
	this->rotation = initRot;
}
sf::Vector2f TransformComponent::getPosition()
{
	return this->position;
}
void TransformComponent::setPosition(sf::Vector2f newPos)
{
	this->position = newPos;
}
float TransformComponent::getRotation()
{
	return this->rotation;
}
void TransformComponent::setRotation(float newRot)
{
	this->rotation = newRot;
	// Ensure the rotation is always between 0 and 2pi
	while (this->rotation < 0)
		this->rotation += (float)(2 * M_PI);
	while (this->rotation > 2 * M_PI)
		this->rotation -= (float)(2 * M_PI);
}

std::pair<sf::Vector2f, float> TransformComponent::getDifference(TransformComponent other)
{
	sf::Vector2f diff = other.getPosition() - this->getPosition();
	// For some reason, the range of atan in c++ is [-pi, pi] instead of [0, 2pi]
	// So the next 3 lines convert it to [0, 2pi]
	float angle = atan2(diff.y, diff.x);
	if (angle < 0)
		angle += (float)(2 * M_PI);
	// Get difference between this entity's rotation and the angle to the other
	angle -= this->getRotation();
	// If the angle is greater than pi, it is shorter to go the negative direction
	if (angle > M_PI)
		angle -= (float)(2 * M_PI);
	else if (angle < -M_PI)
		angle += (float)(2 * M_PI);
	return { diff, angle };

}