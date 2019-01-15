#include "PirateShipController.h"

PirateShipController::PirateShipController() {
	// todo fix this
	this->lastTargetCoords = { 500, 500 };
}
void PirateShipController::buildTrailToTarget(sf::Vector2f pos) {
	if (sf::Vector2i(pos / 64.0f) != sf::Vector2i(this->lastTargetCoords / 64.0f)) {
		this->lastTargetCoords = pos;
		AutomatedShipController::setTarget(pos);
	}
}
bool PirateShipController::isWithinRange(std::weak_ptr<Entity> e, float d) {
	auto diff = this->getParent().lock()->components.transform->getDifference(
		e.lock()->components.transform
	);
	return pow(diff.first.x, 2) + pow(diff.first.y, 2) <= pow(d, 2);
}
