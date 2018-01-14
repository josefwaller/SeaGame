#include "TransformComponent.h"

TransformComponent::TransformComponent(Entity& e, sf::Vector2f initPos = { 0, 0 }, float initRot = 0.0f): Component(e)
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
}