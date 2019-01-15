#pragma once
#include "PirateShipController.h"

/*
 * A pirate ship which stays near and defends a base
 * If the base is destroyed, it will just stay where it is unless chasing a target
 */
class DefensePirateShipController : public PirateShipController {
public:
	// The furthest the ship will be from its base
	// i.e. if it is chasing a target, it will stop when it is this far from its base
	static const float MAX_DISTANCE_TO_BASE;
	// Create with base
	DefensePirateShipController(std::weak_ptr<Entity> base);
	virtual void update(float delta) override;
	virtual SaveData getSaveData() override;
	virtual void fromSaveData(SaveData date) override;
private:
	// The target the ship is currently shooting at
	std::weak_ptr<Entity> target;
};
