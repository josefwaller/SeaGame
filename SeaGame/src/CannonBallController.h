#pragma once
#include "ControllerComponent.h"

class CannonBallController : public ControllerComponent
{
public:
	// How far each cannonball goes before being destroyed
	static const float MAX_DISTANCE;
	// Speed at which cannonballs move
	static const float SPEED;

	CannonBallController(std::weak_ptr<Entity> parent, float angle);
	virtual void update(float delta) override;
	virtual void onCollision(std::weak_ptr<Entity> spawner) override;
private:
	float angle;
	// Distance the cannonball has gone
	float distance;
};