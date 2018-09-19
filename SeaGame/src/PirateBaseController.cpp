#include "PirateBaseController.h"
#include "EntityPrefabs.h"

// 30 seconds
float PirateBaseController::SPAWN_DELAY = 30.0f * 1000.0f;

PirateBaseController::PirateBaseController(std::weak_ptr<Entity> parent): ControllerComponent(parent) {

}

void PirateBaseController::update(float delta) {
	if (this->spawnTimer.getElapsedTime().asMilliseconds() >= SPAWN_DELAY) {
		this->spawnShip();
		this->spawnTimer.restart();
	}
}

void PirateBaseController::spawnShip() {
	this->getParent()->game->addEntity(EntityPrefabs::enemyChasingShip(
		this->getParent()->game,
		this->getParent()->transform->getPosition() + sf::Vector2f(-64.0f, -64.0f),
		ShipRenderer::SAIL_COLOR::Black
	));
}