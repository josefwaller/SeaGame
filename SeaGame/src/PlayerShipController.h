#pragma once
#include "ShipController.h"

class PlayerShipController : public ShipController
{
public:
	PlayerShipController();
	virtual void update(float delta) override;
};