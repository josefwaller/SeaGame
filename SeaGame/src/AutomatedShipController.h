#pragma once
#include "ShipController.h"

// Ship which is controlled by the game
// Mainly contains the moveToPoint method, which moves the ship
// towards a point given
class AutomatedShipController : public ShipController {
public:
	AutomatedShipController(std::weak_ptr<Entity> parent);
	std::vector<sf::Vector2f> points;
protected:
	void move(float delta);
	void setTarget(sf::Vector2f target);
	virtual void onReachingTarget();
private:
	size_t pointsIndex;
};
