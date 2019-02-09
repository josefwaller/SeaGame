#pragma once
#include "ShipController.h"

// Ship which is controlled by the game
// Mainly contains the moveToPoint method, which moves the ship
// towards a point given
class AutomatedShipController : public ShipController {
public:
	std::vector<sf::Vector2f> points;
protected:
	void move(float delta);
	void setTarget(sf::Vector2f target);
	virtual void onReachingTarget();
	// Get the coordinates for reaching an entity
	// Basically if the entity is on land, return its dock coordinates
	sf::Vector2f getCoordsForEntity(std::weak_ptr<Entity> e);
	// The speed at which to move
private:
	size_t pointsIndex;
};
