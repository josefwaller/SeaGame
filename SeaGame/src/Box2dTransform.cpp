#include "Box2dTransform.h"

const float Box2dTransform::LINEAR_DAMPING = 1.0f;
const float Box2dTransform::ANGULAR_DAMPING = 2.0f;

Box2dTransform::Box2dTransform(std::weak_ptr<Entity> parent, b2BodyDef* def, std::vector<b2FixtureDef> fixtures, bool addDamp)
	: TransformComponent(parent)
{
	// Create the body
	this->body = this->getParent().lock()->game->getWorld().lock()->CreateBody(def);
	if (addDamp) {
		this->body->SetLinearDamping(Box2dTransform::LINEAR_DAMPING);
		this->body->SetAngularDamping(Box2dTransform::ANGULAR_DAMPING);
	}
	// Add fixtures
	for (b2FixtureDef def : fixtures) {
		this->body->CreateFixture(&def);
	}
}

sf::Vector2f Box2dTransform::getPosition()
{
	// Convert position to sf::Vector2f
	b2Vec2 pos = this->body->GetPosition();
	return sf::Vector2f(pos.x , pos.y) * Game::METER_TO_PIXEL;
}
void Box2dTransform::setPosition(sf::Vector2f pos)
{
	// Set the body's position
	this->body->SetTransform({ pos.x / Game::METER_TO_PIXEL , pos.y / Game::METER_TO_PIXEL }, this->body->GetAngle());
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
std::map<std::string, std::string> Box2dTransform::getSaveData() {
	return {
		{"x", std::to_string(this->getPosition().x / 100.0f)},
		{"y", std::to_string(this->getPosition().y / 100.0f)},
		{"r", std::to_string(this->getRotation())}
	};
}