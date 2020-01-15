#include "PlayerShipController.h"
#include "EntityPrefabs.h"

PlayerShipController::PlayerShipController() : ShipController()
{

}

void PlayerShipController::update(float delta)
{
	auto x = this->getComponentList().transform->getPosition();
	sf::Vector2f diff = this->getGame()->getMouseCoords() - this->getComponentList().transform->getPosition();
	float angle = atan2(diff.y, diff.x);
	this->aimSwivel(angle);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		this->accelerate();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		this->turnLeft();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		this->turnRight();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		this->shootSwivel();
	}
}

void PlayerShipController::onDeath() {
	// Add the respawn ship
	std::shared_ptr<Entity> respawnShip = EntityPrefabs::respawnPlayerShip(
		this->getGame(),
		this->getComponentList().transform->getPosition()
	);
	this->getGame()->addEntity(respawnShip);
	this->getGame()->setPlayer(respawnShip);
	// Remove self
	this->getGame()->removeEntity(this->getParent());
}