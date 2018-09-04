#pragma once
#include "ShipController.h"

// Ship which is controlled by the game
// Mainly contains the moveToPoint method, which moves the ship
// towards a point given
class AutomatedShipController : public ShipController {
public:
	AutomatedShipController(std::weak_ptr<Entity> parent);
protected:
	void moveToPoint(float delta, sf::Vector2f target);
	virtual void onReachingTarget();
};
