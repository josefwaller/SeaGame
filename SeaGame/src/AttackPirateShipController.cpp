#include "AttackPirateShipController.h"
#include "BaseController.h"
#include <memory>

AttackPirateShipController::AttackPirateShipController(
	std::weak_ptr<Entity> base,
	std::weak_ptr<Entity> target) {
	this->base = base;
	this->target = target;
}
void AttackPirateShipController::update(float delta) {
	if (this->target.lock()) {
		// Check if the target is too far away
		if (!isWithinRange(this->target, MIN_CHASE_DISTANCE)) {
			this->buildTrailToTarget(this->getCoordsForEntity(this->target));
			this->move(delta);
		}
		// Shoot if close enough
		if (isWithinRange(this->target, MAX_CHASE_DISTANCE)) {
			this->aimSwivelAtPoint(this->target.lock()->components.transform->getPosition());
			this->shootSwivel();
		}
	}
	else {
		// Look for target
		float minDis = 0.0f;
		std::weak_ptr<Entity> newTarget;
		std::vector<std::shared_ptr<Entity>> entities = this->getParent().lock()->game->getEntities();
		for (auto it = entities.begin(); it != entities.end(); it++) {
			// Ensure they are on the player's team
			if ((*it)->team == 0) {
				// Get distance to potential target
				sf::Vector2f diff = this->getParent().lock()->components.transform->getDifference(
					(*it)->components.transform
				).first;
				float dis = pow(diff.x, 2) + pow(diff.y, 2);
				// If this is the first entity, set it as newTarget
				// Otherwise make sure it's closer than the current newTarget
				if (newTarget.lock()) {
					if (dis < minDis) {
						minDis = dis;
						newTarget = *it;
					}
				}
				else {
					newTarget = *it;
					minDis = dis;
				}
			}
		}
		if (newTarget.lock()) {
			this->target = newTarget;
		}
	}
}