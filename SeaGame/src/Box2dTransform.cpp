#include "Box2dTransform.h"

Box2dTransform::Box2dTransform(std::weak_ptr<Entity> parent, b2BodyDef* def, std::vector<b2FixtureDef> fixtures)
	: TransformComponent(parent)
{
	// Create the body
	this->body = this->getParent()->game->getWorld().lock()->CreateBody(def);
	// Add fixtures
	for (b2FixtureDef def : fixtures) {
		this->body->CreateFixture(&def);
	}
}

sf::Vector2f Box2dTransform::getPosition()
{
	// Convert position to sf::Vector2f
	b2Vec2 pos = this->body->GetPosition();
	return { pos.x, pos.y };
}
void Box2dTransform::setPosition(sf::Vector2f pos)
{
	// Set the body's position
	this->body->SetTransform({ pos.x, pos.y }, this->body->GetAngle());
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