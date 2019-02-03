#include "Box2dTransform.h"
#include "Box2D/Box2D.h"
#include "Game.h"

const float Box2dTransform::LINEAR_DAMPING = 1.0f;
const float Box2dTransform::ANGULAR_DAMPING = 2.0f;

Box2dTransform::Box2dTransform(b2Body* body)
	: TransformComponent()
{
	// Create the body
	this->body = body;
	this->body->SetUserData(this);

}
Box2dTransform::~Box2dTransform()
{
	// Destory the body
	this->world.lock()->DestroyBody(this->body);
}
void Box2dTransform::setParent(std::weak_ptr<Entity> parent) {
	Component::setParent(parent);
	// Store world
	this->world = this->getGame()->getWorld();
}



sf::Vector2f Box2dTransform::getPosition()
{
	// Convert position to sf::Vector2f
	b2Vec2 pos = this->body->GetPosition();
	return sf::Vector2f(pos.x , pos.y) * Game::BOX2D_TO_WORLD;
}
void Box2dTransform::setPosition(sf::Vector2f pos)
{
	// Set the body's position
	this->body->SetTransform({ pos.x * Game::WORLD_TO_BOX2D , pos.y * Game::WORLD_TO_BOX2D }, this->body->GetAngle());
}
float Box2dTransform::getRotation()
{
	return this->body->GetAngle();
}
void Box2dTransform::setRotation(float angle)
{
	this->body->SetTransform(this->body->GetPosition(), angle);
}
b2Body* Box2dTransform::getBody()
{
	return this->body;
}
