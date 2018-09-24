#include "ChasingShipController.h"
#include <iostream>

const float ChasingShipController::ANGLE_TURN_MARGIN = (float)(M_PI / 8);
const float ChasingShipController::ANGLE_MOVE_MARGIN = (float)M_PI;
const float ChasingShipController::MIN_MOVE_DISTANCE = 300.0f;

ChasingShipController::ChasingShipController(std::weak_ptr<Entity> parent) : ShipController(parent)
{
	// Currently just sets target to player
	this->target = this->getParent().lock()->game->getPlayer();
}

void ChasingShipController::update(float delta)
{
	if (!this->getParent().lock()->game->getPlayer())
		return;
	// Get the transform for easy reference
	auto trans = this->getParent().lock()->transform;
	// Get the difference between this ship's position and the target's position
	std::pair<sf::Vector2f, float> diff = trans->getDifference(
		(this->getParent().lock()->game->getPlayer()->transform)
	);
	// Turn left or right if the ship is not facing the target
	if (diff.second < - ANGLE_TURN_MARGIN) {
		this->turnLeft();
	}
	else if (diff.second > ANGLE_TURN_MARGIN) {
		this->turnRight();
	}
	// Move towards the target if the ship is far away and facing towards the target
	if (sqrt(pow(diff.first.x, 2) + pow(diff.first.y, 2)) > MIN_MOVE_DISTANCE && abs(diff.second) < ANGLE_MOVE_MARGIN) {
		this->accelerate();
	}
	// Aim the swivel at the target
	this->aimSwivel(diff.second + this->getParent().lock()->transform->getRotation());
	this->shootSwivel();
}