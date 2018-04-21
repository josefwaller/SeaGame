#include "CannonBallController.h"
#include "EntityPrefabs.h"
#include "PhysicsComponent.h"

const float CannonBallController::SPEED = 300.0f;
const float CannonBallController::MAX_DISTANCE = 500.0f;
CannonBallController::CannonBallController(std::weak_ptr<Entity> parent, float angle)
	: ControllerComponent(parent)
{
	this->angle = angle;
	this->distance = 0;
}

void CannonBallController::update(float delta)
{
	// Move the cannonball
	auto physics = this->getParent()->physics;
	physics->setVelocity({ SPEED * cos(this->angle), SPEED * sin(this->angle) });
	// Check if it has gone far enough and should be destroyed
	this->distance += delta * SPEED;
	if (this->distance >= MAX_DISTANCE) {
		this->getParent()->game->removeEntity(this->getParent());
	}

}
void CannonBallController::onCollision(std::weak_ptr<Entity> other)
{
	if (other.lock()) {
		if (other.lock()->team != this->getParent()->team) {
			// Add explosion
			this->getParent()->game->addEntity(EntityPrefabs::explosion(
				this->getParent()->game,
				this->getParent()->transform->getPosition()));
			// Damage other entity
			if (other.lock()->controller != nullptr)
				other.lock()->controller->onHit(HealthType::Sails, 10);
			// Remove self
			this->getParent()->game->removeEntity(this->getParent());
		}
	}
}