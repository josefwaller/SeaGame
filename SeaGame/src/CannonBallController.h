#pragma once
#include "ControllerComponent.h"

class CannonBallController : public ControllerComponent
{
public:
	CannonBallController(std::weak_ptr<Entity> parent);
	virtual void update(float delta) override;
	// Speed at which cannonballs move
	static const float SPEED;
};