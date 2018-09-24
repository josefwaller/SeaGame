#include "ChasingShipController.h"
#include <iostream>

const float ChasingShipController::ANGLE_TURN_MARGIN = (float)(M_PI / 8);
const float ChasingShipController::ANGLE_MOVE_MARGIN = (float)M_PI;
const float ChasingShipController::MIN_MOVE_DISTANCE = 300.0f;
const float ChasingShipController::MAX_CHASE_RANGE = 500.0f;

ChasingShipController::ChasingShipController(std::weak_ptr<Entity> parent) : ShipController(parent)
{
}

void ChasingShipController::update(float delta)
{
	if (this->target.lock()) {
		// Get the transform for easy reference
		auto trans = this->getParent().lock()->transform;
		// Get the difference between this ship's position and the target's position
		std::pair<sf::Vector2f, float> diff = trans->getDifference(
			(this->target.lock()->transform)
		);
		// Turn left or right if the ship is not facing the target
		if (diff.second < -ANGLE_TURN_MARGIN) {
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
	else {
		// No target, look for one
		for (auto it : this->getParent().lock()->game->getEntities()) {
			// Skip potential targets on its team
			if (it->team == this->getParent().lock()->team)
				continue;
			// Check how close the entity is
			sf::Vector2f diff = this->getParent().lock()->transform->getDifference(
				it->transform->getPosition()
			).first;
			if (sqrt(pow(diff.x, 2) + pow(diff.y, 2)) < ChasingShipController::MAX_CHASE_RANGE) {
				this->setTarget(it);
				break;
			}
		}
	}
}
void ChasingShipController::setTarget(std::weak_ptr<Entity> other) {
	this->target = other;
}