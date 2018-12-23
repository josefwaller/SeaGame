#pragma once
#include "ControllerComponent.h"

class CannonballController : public ControllerComponent
{
public:
	// How far each cannonball goes before being destroyed
	static const float MAX_DISTANCE;
	// Speed at which cannonballs move
	static const float SPEED;

	CannonballController(float angle);
	virtual void update(float delta) override;
	virtual void onCollision(std::weak_ptr<Entity> spawner) override;
private:
	float angle;
	// Original position of the cannonball
	sf::Vector2f startPos;
};