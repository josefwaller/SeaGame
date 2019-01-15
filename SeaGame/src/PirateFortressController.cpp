#include "PirateFortressController.h"
#include "EntityPrefabs.h"

const float PirateFortressController::SPAWN_DELAY = 30.0f * 1000.0f;
PirateFortressController::PirateFortressController() {}
void PirateFortressController::update(float delta) {
	if (this->ship.lock()) {
		this->spawnTimer.restart();
	}
	else {
		if (this->spawnTimer.getElapsedTime().asMilliseconds() >= SPAWN_DELAY) {
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
			std::shared_ptr<Entity> ship = EntityPrefabs::attackPirateShip(
				this->getParent().lock()->game,
				this->getDockCoords(),
				this->getParent(),
				newTarget);
			this->getParent().lock()->game->addEntity(ship);
			this->ship = ship;
		}
	}
}
