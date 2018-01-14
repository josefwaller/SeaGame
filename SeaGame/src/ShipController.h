#pragma once
#include "ControllerComponent.h"

// Control the player's ship using keyboard input
class ShipController : public ControllerComponent
{
public:
	ShipController(Entity& e);
	// Set the angular velocity to turn either left or right when move() is called
	void turnLeft();
	void turnRight();
	// Set the accereration to move forward when move() is called
	void accelerate();
	// Apply movements to the entity based on velocity and angular velocity
	void move(float delta);
	// Max speed and turn speed a ship can be moving at
	const static float MAX_VELOCITY;
	static const float MAX_ANGULAR_VELOCITY;
	// The acceration of all ship's turning/moving
	const static float ACCELERATION;
	const static float ANGULAR_ACCELERATION;
private:
	// Velocity and angular velocity of the ship
	float velocity;
	float angularVelocity;
	// Current acceleration and angular acceleration of the ship
	// Set by either turnRight(), turnLeft() or accelerate()
	float acceleration;
	float angularAcceleration;
};