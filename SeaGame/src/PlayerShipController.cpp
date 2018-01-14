#include "PlayerShipController.h"

PlayerShipController::PlayerShipController(Entity& e) : ShipController(e)
{

}

void PlayerShipController::update(float delta)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		this->accelerate();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		this->turnLeft();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		this->turnRight();
	}
	this->move(delta);
}