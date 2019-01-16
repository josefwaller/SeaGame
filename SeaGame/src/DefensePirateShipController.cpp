#include "DefensePirateShipController.h"

const float DefensePirateShipController::MAX_DISTANCE_TO_BASE = 500.0f;
const float DefensePirateShipController::MAX_CHASE_DISTANCE = 500.0f;
const float DefensePirateShipController::MIN_CHASE_DISTANCE = 250.0f;

DefensePirateShipController::DefensePirateShipController(std::weak_ptr<Entity> base) {
	this->base = base;
}
void DefensePirateShipController::update(float delta) {
	/*
	 * If the ship has a target:
	 *	if the ship is close enough to the base or there is no base
	 *		Move to the target
	 *	if the target is in range
	 *		Shoot at the target
	 *	else
	 *		Reset target
	 * else
	 *	If the ship has a base
	 *		If it's close enough to the base
	 *			do nothing
	 *		else
	 *			Move to base
	 */
	if (this->target.lock()) {
		if (!this->base.lock()) {
			// If it doesn't have a base, just chase the target
			this->buildTrailToTarget(this->getCoordsForEntity(this->target));
			this->move(delta);
		}
		else {
			// Shoot at target
			this->aimSwivelAtPoint(this->target.lock()->components.transform->getPosition());
			this->shootSwivel();
			// Check if it's close enough to see
			if (isWithinRange(this->target, MAX_CHASE_DISTANCE)) {
				// Check if it's far enough from the ship to chase them
				if (!isWithinRange(this->target, MIN_CHASE_DISTANCE)) {
					// Check if it's close enough to the base
					if (isWithinRange(this->base, MAX_DISTANCE_TO_BASE)) {
						// Move towards them
						this->buildTrailToTarget(this->getCoordsForEntity(this->target));
						this->move(delta);
					}
				}
			}
			else {
				this->target.reset();
			}
		}
	}
	else {
		// Ensure the base is within range
		if (this->base.lock()) {
			if (!isWithinRange(this->base, MAX_DISTANCE_TO_BASE)) {
				this->buildTrailToTarget(this->getCoordsForEntity(this->base));
				this->move(delta);
			}
			else {
				// Look for target
				this->target = this->findTarget();
			}
		}
		else {
			// If there is no base, just find a target without worrying about going too far from the base
			this->target = this->findTarget();
		}
	}
}
std::weak_ptr<Entity> DefensePirateShipController::findTarget() {
	for (auto e : this->getGame()->getEntities()) {
		if (e->team == 0) {
			if (isWithinRange(e, MAX_CHASE_DISTANCE)) {
				return e;
			}
		}
	}
	return std::weak_ptr<Entity>();
}
SaveData DefensePirateShipController::getSaveData() {
	if (this->base.lock()) {
		return SaveData("Component", {
			{ "hasBase", std::to_string(true) },
			{ "baseId", std::to_string(this->base.lock()->id) }
		});
	}
	else {
		return SaveData("Component", {
			{ "hasBase", std::to_string(false) }
		});
	}
}
void DefensePirateShipController::fromSaveData(SaveData data) {
	if ((bool)std::stoi(data.getValue("hasBase"))) {
		this->base = this->getGame()->getEntityById(
			std::stoi(data.getValue("baseId"))
		);
	}
}