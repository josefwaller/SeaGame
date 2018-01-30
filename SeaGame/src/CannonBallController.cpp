#include "CannonBallController.h"
#include "EntityPrefabs.h"

const float CannonBallController::SPEED = 300.0f;
CannonBallController::CannonBallController(std::weak_ptr<Entity> parent, std::weak_ptr<Entity> spawner)
	: ControllerComponent(parent)
{
	this->spawner = spawner;
}

void CannonBallController::update(float delta)
{
	auto trans = this->getParent()->transform;
	trans->setPosition(trans->getPosition()
		+ sf::Vector2f(
			CannonBallController::SPEED * cos(trans->getRotation()) * delta,
			CannonBallController::SPEED * sin(trans->getRotation()) * delta
		)
	);
}
void CannonBallController::onCollision(std::weak_ptr<Entity> other)
{
	if (other.lock() != this->spawner.lock()) {
		this->getParent()->game->addEntity(EntityPrefabs::explosion(
			this->getParent()->game,
			this->getParent()->transform->getPosition()));
		this->getParent()->game->removeEntity(this->getParent());
	}
}