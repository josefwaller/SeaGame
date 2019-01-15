#include "PirateFortressController.h"
#include "EntityPrefabs.h"

const float PirateFortressController::SPAWN_DELAY = 5.0f * 1000.0f;
PirateFortressController::PirateFortressController() {}
void PirateFortressController::update(float delta) {
	if (this->spawnTimer.getElapsedTime().asMilliseconds() >= SPAWN_DELAY) {
		this->spawnTimer.restart();
		this->getParent().lock()->game->addEntity(EntityPrefabs::attackPirateShip(
			this->getParent().lock()->game,
			this->getParent().lock()->components.transform->getPosition() - sf::Vector2f(64.0f, 64.0f),
			this->getParent()));
	}
}
