#include "ShipController.h"
#include <iostream>

const float ShipController::MAX_VELOCITY = 100.0f;
const float ShipController::MAX_ANGULAR_VELOCITY = 1.0f;

const float ShipController::ACCELERATION = 100.0f;
const float ShipController::ANGULAR_ACCELERATION = 2.0f;

const float ShipController::IDLE_DECCELERATION = 500.0f;
const float ShipController::IDLE_ANGULAR_DECELLERATION = 30.0f;

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
	// Set current velocity
	this->velocity += this->acceleration * delta;
	// Ensure that velocity is not above the maximum allowed
	if (this->velocity > ShipController::MAX_VELOCITY) {
		this->velocity = ShipController::MAX_VELOCITY;
	}
	// Set current angular velocity
	this->angularVelocity += this->angularAcceleration * delta;
	// Ensure that angular velocity is not too high
	if (this->angularVelocity > ShipController::MAX_ANGULAR_VELOCITY) {
		this->angularVelocity = ShipController::MAX_ANGULAR_VELOCITY;
	}
	else if (this->angularVelocity < -ShipController::MAX_ANGULAR_VELOCITY) {
		this->angularVelocity = -ShipController::MAX_ANGULAR_VELOCITY;
	}
	// Apply movements to this entity's transform
	auto transform = this->getParent().transform;
	transform->setPosition(
		transform->getPosition() + sf::Vector2f(
			this->velocity * cos(transform->getRotation()) * delta,
			this->velocity * sin(transform->getRotation()) * delta
		)
	);
	transform->setRotation(transform->getRotation() + this->angularVelocity * delta);
	// Set velocity for next frame
	// Values set here may be overridden by turnLeft(), turnRight() or other methods
	// These are the defaults in case it is not
	if (this->velocity > 0)
	{
		// Temp, will change to a new const
		this->acceleration = -ShipController::IDLE_DECCELERATION;
	}
	else
	{
		// Currently cannot go backwards
		this->velocity = 0;
	}
	// Slow down the ship's turning
	if (this->angularVelocity > 0.01)
	{
		this->angularAcceleration = -ShipController::IDLE_ANGULAR_DECELLERATION;
	}
	else if (this->angularVelocity < -0.01)
	{
		this->angularAcceleration = ShipController::IDLE_ANGULAR_DECELLERATION;
	}
	else
	{
		// Stop turning if turning very slowly
		this->angularAcceleration = 0.0f;
	}
}