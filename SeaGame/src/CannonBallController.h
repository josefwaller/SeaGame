#pragma once
#include "ControllerComponent.h"

class CannonBallController : public ControllerComponent
{
public:
	CannonBallController(std::weak_ptr<Entity> parent, float angle, std::weak_ptr<Entity> spawner);
	virtual void update(float delta) override;
	virtual void onCollision(std::weak_ptr<Entity> spawner) override;
	// Speed at which cannonballs move
	static const float SPEED;
private:
	// Ship which shot the cannonball
	std::weak_ptr<Entity> spawner;
	float angle;
};