#include "CannonBallController.h"

const float CannonBallController::SPEED = 300.0f;
CannonBallController::CannonBallController(std::weak_ptr<Entity> parent) : ControllerComponent(parent)
{
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