#pragma once
#include "AutomatedShipController.h"

/*
 * A pirate ship which stays near and defends a base
 * If the base is destroyed, it will just stay where it is unless chasing a target
 */
class DefensePirateShipController : public AutomatedShipController {
public:
	// The furthest the ship will be from its base
	// i.e. if it is chasing a target, it will stop when it is this far from its base
	static const float MAX_DISTANCE_TO_BASE;
	// The distance at which it can see a potential target and start chasing them
	static const float MAX_CHASE_DISTANCE;
	// The distance at which it will stop chasing a target and just shoot at them
	static const float MIN_CHASE_DISTANCE;
	// Create with base
	DefensePirateShipController(std::weak_ptr<Entity> base);
	// Set target such that it will only recalculate A* path if the target has moved far enough away
	void setTarget(sf::Vector2f pos);
	virtual void update(float delta) override;
	virtual SaveData getSaveData() override;
	virtual void fromSaveData(SaveData date) override;
private:
	// The base the ship is defending
	std::weak_ptr<Entity> base;
	// The target the ship is currently shooting at
	std::weak_ptr<Entity> target;
	// The previous target coords
	sf::Vector2f lastTargetCoords;
	// Whether the entity is within the range given
	// The entity must not be nullptr and must have a transform component
	bool isWithinRange(std::weak_ptr<Entity> e, float dis);
};
