#include "ShipController.h"
#include "EntityPrefabs.h"
#include <iostream>

const float ShipController::ACCELERATION = 300.0f;
const float ShipController::ANGULAR_ACCELERATION = 2.0f;

const float ShipController::CANNON_INTERVAL = 2.0f;

ShipController::ShipController(std::weak_ptr<Entity> e) : ControllerComponent(e)
{
	this->physicsComp = this->getParent().lock()->physics;
	this->cannon = Cannon(
		this->getParent(),
		sf::Vector2f(0.0f, 0.0f),
		0,
		CANNON_INTERVAL);
}

void ShipController::turnLeft()
{
	this->physicsComp.lock()->setAngularAcceleration(-ShipController::ANGULAR_ACCELERATION);
}

void ShipController::turnRight()
{
	this->physicsComp.lock()->setAngularAcceleration(ShipController::ANGULAR_ACCELERATION);
}
void ShipController::accelerate()
{
	this->physicsComp.lock()->setAcceleration({
		ShipController::ACCELERATION * cos(this->getParent().lock()->transform->getRotation()),
		ShipController::ACCELERATION * sin(this->getParent().lock()->transform->getRotation())
	});
}
void ShipController::aimSwivel(float angle)
{
	this->cannon.rotation = angle;
	this->getParent().lock()->renderer->reset();
}
float ShipController::getSwivelAngle()
{
	return this->cannon.rotation;
}
void ShipController::shootSwivel()
{
	this->cannon.fire();
}
void ShipController::onHit(HealthType t, int damageAmount)
{
	if (this->getParent().lock()->health != nullptr) {
		this->getParent().lock()->health->takeDamage(t, damageAmount);
		this->getParent().lock()->renderer->reset();
	}
}
void ShipController::onDeath() {
	this->getParent().lock()->game->removeEntity(this->getParent().lock());
	this->getParent().lock()->game->addEntity(EntityPrefabs::explosion(
		this->getParent().lock()->game,
		this->getParent().lock()->transform->getPosition()
	));
}