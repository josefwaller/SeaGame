#include "ShipController.h"
#include "EntityPrefabs.h"
#include <iostream>

const float ShipController::ACCELERATION = 300.0f;
const float ShipController::ANGULAR_ACCELERATION = 2.0f;

const float ShipController::CANNON_INTERVAL = 2.0f;

ShipController::ShipController(std::weak_ptr<Entity> e) : ControllerComponent(e)
{
	this->physicsComp = this->getParent()->physics;
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
		ShipController::ACCELERATION * cos(this->getParent()->transform->getRotation()),
		ShipController::ACCELERATION * sin(this->getParent()->transform->getRotation())
	});
}
void ShipController::aimSwivel(float angle)
{
	this->swivelCannonAngle = angle;
	this->getParent()->renderer->reset();
}
float ShipController::getSwivelAngle()
{
	return this->swivelCannonAngle;
}
void ShipController::shootSwivel()
{
	if (this->swivelCannonClock.getElapsedTime().asSeconds() > ShipController::CANNON_INTERVAL) {
		this->swivelCannonClock.restart();
		this->getParent()->game->addEntity(
			EntityPrefabs::cannonBall(
				this->getParent()->game,
				this->getParent(),
				this->getParent()->transform->getPosition(),
				this->swivelCannonAngle
			)
		);
	}
}
void ShipController::onHit(HealthType t, int damageAmount)
{
	if (this->getParent()->health != nullptr) {
		this->getParent()->health->takeDamage(t, damageAmount);
		this->getParent()->renderer->reset();
	}
}