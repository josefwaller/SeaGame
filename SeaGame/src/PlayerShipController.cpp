#include "PlayerShipController.h"

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