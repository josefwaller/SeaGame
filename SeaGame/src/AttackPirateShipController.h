#pragma once
#include "PirateShipController.h"
// Forward Declaration
class SaveData;

/*
 * Attack one of the player's bases, then keep attacking the nearest entity on the
 * player's team until death
 */
class AttackPirateShipController : public PirateShipController {
public:
	// The distance at which the ship will stop and just shoot at the target
	const static float SHOOT_DISTANCE;
	AttackPirateShipController(std::weak_ptr<Entity> base, std::weak_ptr<Entity> target);
	virtual void update(float delta) override;
	virtual SaveData getSaveData() override;
	virtual void fromSaveData(SaveData data) override;
private:
	// Current target
	std::weak_ptr<Entity> target;
	// Base, to tell when the target is destroyed
	std::weak_ptr<Entity> base;
};
