#pragma once
#include "ControllerComponent.h"

// Control the player's ship using keyboard input
class ShipController : public ControllerComponent
{
public:
	ShipController(std::weak_ptr<Entity> e);
	// Set the angular velocity to turn either left or right when move() is called
	void turnLeft();
	void turnRight();
	// Set the accereration to move forward when move() is called
	void accelerate();
	// Aim the swivel in a certain direction
	void aimSwivel(float angle);
	// Get the angle the swivel is pointing at
	float getSwivelAngle();
	// Shoot the main swivel cannon in the direction specified
	void shootSwivel();
	// Apply movements to the entity based on velocity and angular velocity
	void move(float delta);
	// Max speed and turn speed a ship can be moving at
	const static float MAX_VELOCITY;
	static const float MAX_ANGULAR_VELOCITY;
	// The acceration of all ship's turning/moving
	const static float ACCELERATION;
	const static float ANGULAR_ACCELERATION;
	// How quickly the ship slows down when not being actively accelerated
	const static float IDLE_DECCELERATION;
	// How quickly the ship stops turning when not actively turning
	const static float IDLE_ANGULAR_DECELLERATION;
	// How long the ship has to wait between shooting
	const static float CANNON_INTERVAL;
private:
	// Velocity and angular velocity of the ship
	float velocity;
	float angularVelocity;
	// Current acceleration and angular acceleration of the ship
	// Set by either turnRight(), turnLeft() or accelerate()
	float acceleration;
	float angularAcceleration;
	// Angle the swivel cannon is pointing at
	// Note: if the ship does not have a swivel cannon, will be ignored
	float swivelCannonAngle;
	// Count time since the last swivel cannon was fired
	sf::Clock swivelCannonClock;
};