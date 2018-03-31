#include "PlayerShipController.h"

PlayerShipController::PlayerShipController(std::weak_ptr<Entity> e) : ShipController(e)
{

}

void PlayerShipController::update(float delta)
{
	sf::Vector2f diff = this->getParent()->game->getMouseCoords() - this->getParent()->transform->getPosition();
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
		auto x = 0;
	}
}