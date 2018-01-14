#include "ShipController.h"

const float ShipController::MAX_VELOCITY = 10.0f;
const float ShipController::MAX_ANGULAR_VELOCITY = 30.0f;

const float ShipController::ACCELERATION = 1000.0f;
const float ShipController::ANGULAR_ACCELERATION = 100.0f;

ShipController::ShipController(Entity& e) : ControllerComponent(e)
{
	this->acceleration = 0.0f;
	this->angularAcceleration = 0.0f;
	this->velocity = 0.0f;
	this->angularVelocity = 0.0f;
}

void ShipController::turnLeft()
{
	this->angularAcceleration = - ShipController::ANGULAR_ACCELERATION;
}

void ShipController::turnRight()
{
	this->angularAcceleration = ShipController::ANGULAR_ACCELERATION;
}
void ShipController::accelerate()
{
	this->acceleration = ShipController::ACCELERATION;
}

void ShipController::move(float delta)
{
	this->velocity += this->acceleration * delta;
	this->angularVelocity += this->angularAcceleration * delta;

	auto transform = this->getParent().transform;
	transform->setPosition(
		transform->getPosition() + sf::Vector2f(
			this->velocity * cos(transform->getRotation()) * delta,
			this->velocity * sin(transform->getRotation()) * delta
		)
	);
	transform->setRotation(transform->getRotation() + this->angularVelocity * delta);
	if (this->velocity > 0)
	{
		// Temp, will change to a new const
		this->acceleration = -ShipController::ACCELERATION / 2;
	}
	else
	{
		// Currently cannot go backwards
		this->velocity = 0;
	}
	// Slow down the ship's turning
	if (this->angularVelocity > 0.01)
	{
		this->angularAcceleration = -ShipController::ANGULAR_ACCELERATION / 2;
	}
	else if (this->angularVelocity < -0.01)
	{
		this->angularAcceleration = ShipController::ANGULAR_ACCELERATION / 2;
	}
	else
	{
		// Stop turning if turning very slowly
		this->angularAcceleration = 0.0f;
	}
}