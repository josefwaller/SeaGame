#include "RespawnPlayerShipController.h"
#include "EntityPrefabs.h"

const float RespawnPlayerShipController::RESPAWN_DELAY = 10 * 1000.0f;
void RespawnPlayerShipController::update(float delta) {
	if (this->c.getElapsedTime().asMilliseconds() > RESPAWN_DELAY) {
		std::shared_ptr<Entity> player = EntityPrefabs::playerShip(
			this->getGame(),
			this->getComponentList().transform->getPosition()
		);
		this->getGame()->addEntity(player);
		this->getGame()->setPlayer(player);
		this->getGame()->removeEntity(this->getParent());
	}
	else {
		PlayerShipController::update(delta);
	}
}