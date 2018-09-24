#include "AutomatedShipController.h"
#include <math.h>

AutomatedShipController::AutomatedShipController(std::weak_ptr<Entity> parent) : ShipController(parent) {

}
void AutomatedShipController::moveToPoint(float delta, sf::Vector2f target) {
	auto trans = this->getParent().lock()->transform;
	sf::Vector2f difference = target - trans->getPosition();
	float angle = atan2(difference.y, difference.x);
	trans->setRotation(angle);
	if (sqrt(difference.x * difference.x + difference.y * difference.y) > 200) {
		this->accelerate();
	}
	else {
		this->onReachingTarget();
	}
}
// Empty, may be filled in by subclass
void AutomatedShipController::onReachingTarget() {

}