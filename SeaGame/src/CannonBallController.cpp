#include "CannonballController.h"
#include "EntityPrefabs.h"
#include "PhysicsComponent.h"
#include "Entity.h"

const float CannonballController::SPEED = 500.0f;
const float CannonballController::MAX_DISTANCE = 1500.0f;
CannonballController::CannonballController(float angle)
	: ControllerComponent()
{
	this->angle = angle;
}
void CannonballController::setParent(std::weak_ptr<Entity> parent) {
	Component::setParent(parent);
	this->startPos = this->getComponentList().transform->getPosition();
	// Move the cannonball
	auto physics = this->getComponentList().physics;
	physics->setVelocity({
		SPEED * cos(this->angle) * Game::WORLD_TO_BOX2D,
		SPEED * sin(this->angle) * Game::WORLD_TO_BOX2D
	});
}

void CannonballController::update(float delta)
{
	// Check if it has gone far enough and should be destroyed
	sf::Vector2f pos = this->getComponentList().transform->getPosition();
	if (pow(pos.x - this->startPos.x, 2) + pow(pos.y - this->startPos.y, 2) >= pow(MAX_DISTANCE, 2)) {
		this->getGame()->removeEntity(this->getParent().lock());
	}

}
void CannonballController::onCollision(std::weak_ptr<Entity> other)
{
	if (other.lock()) {
		if (other.lock()->team != this->getParent().lock()->team) {
			// Add explosion
			this->getGame()->addEntity(EntityPrefabs::explosion(
				this->getGame(),
				this->getComponentList().transform->getPosition()));
			// Damage other entity
			if (other.lock()->components.controller != nullptr)
				other.lock()->components.controller->onHit(HealthType::Sails, 10);
			// Remove self
			this->getGame()->removeEntity(this->getParent().lock());
		}
	}
}