#pragma once
#include "ShipController.h"

// Ship which is controlled by the game
// Mainly contains the moveToPoint method, which moves the ship
// towards a point given
class AutomatedShipController : public ShipController {
public:
	std::vector<sf::Vector2f> points;
protected:
	void move(float delta, float speed = ShipController::DEFAULT_ACCELERATION);
	void setTarget(sf::Vector2f target);
	// When the ship reaches the target
	virtual void onReachingTarget();
	// When there is no path to the target
	virtual void onNoPath();
	// Get the coordinates for reaching an entity
	// Basically if the entity is on land, return its dock coordinates
	sf::Vector2f getCoordsForEntity(std::weak_ptr<Entity> e);
private:
	size_t pointsIndex;
	// Used to check every second if there is a path available
	sf::Clock noPathClock;
};
