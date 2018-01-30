#pragma once
#include "ShipController.h"

class PlayerShipController : public ShipController
{
public:
	PlayerShipController(std::weak_ptr<Entity> e);
	virtual void update(float delta) override;
};