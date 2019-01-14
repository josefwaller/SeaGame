#include "ShipController.h"
#include "EntityPrefabs.h"
#include <iostream>

const float ShipController::ACCELERATION = 300.0f;
const float ShipController::ANGULAR_ACCELERATION = 2.0f;

const float ShipController::CANNON_INTERVAL = 0.5f;

void ShipController::setParent(std::weak_ptr<Entity> parent)
{
	Component::setParent(parent);
	this->physicsComp = this->getParent().lock()->components.physics;
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
		ShipController::ACCELERATION * cos(this->getParent().lock()->components.transform->getRotation()) * Game::WORLD_TO_BOX2D,
		ShipController::ACCELERATION * sin(this->getParent().lock()->components.transform->getRotation()) * Game::WORLD_TO_BOX2D
	});
}
void ShipController::aimSwivel(float angle)
{
	this->cannon.rotation = angle;
	this->getParent().lock()->components.renderer->reset();
}
void ShipController::aimSwivelAtPoint(sf::Vector2f p) {
	auto trans = this->getParent().lock()->components.transform;
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
	if (this->getParent().lock()->components.health != nullptr) {
		this->getParent().lock()->components.health->takeDamage(damageAmount);
		this->getParent().lock()->components.renderer->reset();
	}
}
void ShipController::onDeath() {
	this->getParent().lock()->game->removeEntity(this->getParent().lock());
	this->getParent().lock()->game->addEntity(EntityPrefabs::explosion(
		this->getParent().lock()->game,
		this->getParent().lock()->components.transform->getPosition()
	));
}