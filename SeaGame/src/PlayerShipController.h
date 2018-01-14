#pragma once
#include "ShipController.h"

class PlayerShipController : public ShipController
{
public:
	PlayerShipController(Entity& e);
	virtual void update(float delta) override;
};