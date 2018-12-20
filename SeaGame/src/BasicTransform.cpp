#include "BasicTransform.h"
#include <iostream>

BasicTransform::BasicTransform(sf::Vector2f initPos = { 0, 0 }, float initRot = 0.0f)
	: TransformComponent()
{
	this->position = initPos;
	this->rotation = initRot;
}
sf::Vector2f BasicTransform::getPosition()
{
	return this->position;
}
void BasicTransform::setPosition(sf::Vector2f newPos)
{
	this->position = newPos;
}
float BasicTransform::getRotation()
{
	return this->rotation;
}
void BasicTransform::setRotation(float newRot)
{
	this->rotation = newRot;
	// Ensure the rotation is always between 0 and 2pi
	while (this->rotation < 0)
		this->rotation += (float)(2 * M_PI);
	while (this->rotation > 2 * M_PI)
		this->rotation -= (float)(2 * M_PI);
}
