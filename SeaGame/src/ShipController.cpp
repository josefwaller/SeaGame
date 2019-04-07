#include "ShipController.h"
#include "EntityPrefabs.h"
#include <iostream>

const float ShipController::DEFAULT_ACCELERATION = 300.0f;
const float ShipController::ANGULAR_ACCELERATION = 2.0f;

const float ShipController::CANNON_INTERVAL = 0.5f;

void ShipController::setParent(std::weak_ptr<Entity> parent)
{
	Component::setParent(parent);
	this->physicsComp = this->getComponentList().physics;
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
void ShipController::accelerate(float a)
{
	this->physicsComp.lock()->setAcceleration({
		a * cos(this->getComponentList().transform->getRotation()) * Game::WORLD_TO_BOX2D,
		a * sin(this->getComponentList().transform->getRotation()) * Game::WORLD_TO_BOX2D
	});
}
void ShipController::aimSwivel(float angle)
{
	this->cannon.rotation = angle;
	this->getComponentList().renderer->reset();
}
void ShipController::aimSwivelAtPoint(sf::Vector2f p) {
	auto trans = this->getComponentList().transform;
	this->aimSwivel(
		trans->getDifference(p).second
		+ trans->getRotation()
	);
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
	if (this->getComponentList().health != nullptr) {
		this->getComponentList().health->takeDamage(damageAmount);
		this->getComponentList().renderer->reset();
	}
}
void ShipController::onDeath() {
	this->getGame()->removeEntity(this->getParent().lock());
	this->getGame()->addEntity(EntityPrefabs::explosion(
		this->getGame(),
		this->getComponentList().transform->getPosition()
	));
}