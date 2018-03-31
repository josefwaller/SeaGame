#include "CannonBallController.h"
#include "EntityPrefabs.h"
#include "PhysicsComponent.h"

const float CannonBallController::SPEED = 300.0f;
CannonBallController::CannonBallController(std::weak_ptr<Entity> parent, float angle, std::weak_ptr<Entity> spawner)
	: ControllerComponent(parent)
{
	this->spawner = spawner;
	this->angle = angle;
}

void CannonBallController::update(float delta)
{
	auto physics = this->getParent()->physics;
	physics->setVelocity({ SPEED * cos(this->angle), SPEED * sin(this->angle) });
}
void CannonBallController::onCollision(std::weak_ptr<Entity> other)
{
	if (other.lock() != this->spawner.lock()) {
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