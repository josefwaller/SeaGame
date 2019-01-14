#pragma once
#include "ControllerComponent.h"
#include "PhysicsComponent.h"
#include "Cannon.h"

// Control the player's ship using keyboard input
class ShipController : public ControllerComponent
{
public:
	virtual void setParent(std::weak_ptr<Entity> override);
	// Set the angular velocity to turn either left or right when move() is called
	void turnLeft();
	void turnRight();
	// Set the accereration to move forward when move() is called
	void accelerate();
	// Aim the swivel in a certain direction
	void aimSwivel(float angle);
	// Aim the swivel at a point
	void aimSwivelAtPoint(sf::Vector2f p);
	// Get the angle the swivel is pointing at
	float getSwivelAngle();
	// Shoot the main swivel cannon in the direction specified
	void shootSwivel();
	virtual void onHit(HealthType t, int damageAmount) override;
	virtual void onDeath() override;
	// The acceration of all ship's turning/moving
	const static float ACCELERATION;
	const static float ANGULAR_ACCELERATION;
	// How long the ship has to wait between shooting
	const static float CANNON_INTERVAL;
private:
	std::weak_ptr<PhysicsComponent> physicsComp;
	// Cannon on the ship
	// If the ship does not have a cannon, will be unused
	Cannon cannon;
};