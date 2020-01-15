#pragma once
#include "AutomatedShipController.h"

/*
 * Parent class for different types of pirate ships
 */
class PirateShipController : public AutomatedShipController {
public:
	PirateShipController();
	// The distance at which it can see a potential target and start chasing them
	static const float MAX_CHASE_DISTANCE;
	// The distance at which it will stop chasing a target and just shoot at them
	static const float MIN_CHASE_DISTANCE;
protected:
	// Whether the entity is within the range given
	// The entity must not be nullptr and must have a transform component
	bool isWithinRange(std::weak_ptr<Entity> e, float dis);
	// Build a trail to the position given
	void buildTrailToTarget(sf::Vector2f pos);
	// The base that spawned this pirate ship
	std::weak_ptr<Entity> base;
	// The target the ship is currently chasing
	std::weak_ptr<Entity> target;
private:
	// The previous target coords
	sf::Vector2f lastTargetCoords;
};
