#include "PirateBaseController.h"
#include "EntityPrefabs.h"

// 30 seconds
float PirateBaseController::SPAWN_DELAY = 3.0f * 1000.0f;
// 1 ship
unsigned int PirateBaseController::MAX_SHIPS = 1;

PirateBaseController::PirateBaseController(): BaseController() {

}

void PirateBaseController::update(float delta) {
	if (this->spawnTimer.getElapsedTime().asMilliseconds() >= SPAWN_DELAY && this->numShips < MAX_SHIPS) {
		this->spawnShip();
		this->numShips++;
		this->spawnTimer.restart();
	}
}

void PirateBaseController::spawnShip() {
	this->getGame()->addEntity(EntityPrefabs::defensePirateShip(
		this->getGame(),
		this->getComponentList().transform->getPosition() + sf::Vector2f(-64.0f, -64.0f),
		this->getParent()
	));
}