#include "FerryShipController.h"

FerryShipController::FerryShipController(std::weak_ptr<Entity> parent, std::weak_ptr<Entity> from, std::weak_ptr<Entity> to)
	: AutomatedShipController(parent) {
	this->setTargets(from, to);
}
void FerryShipController::update(float delta) {
	switch (this->currentAction) {
	case Action::PickingUp:
		this->moveToPoint(delta, this->takeFrom.lock()->transform->getPosition());
		break;
	case Action::DropingOff:
		this->moveToPoint(delta, this->giveTo.lock()->transform->getPosition());
		break;
	}
}
void FerryShipController::setTargets(std::weak_ptr<Entity> takeFrom, std::weak_ptr<Entity> givesTo) {
	this->takeFrom = takeFrom;
	this->giveTo = givesTo;
}

void FerryShipController::onReachingTarget() {
	if (this->currentAction == Action::PickingUp) {
		// Take all of the base's things
		auto inventory = this->takeFrom.lock()->inventory->getInventory();
		for (auto it = inventory.begin(); it != inventory.end(); it++) {
			this->takeFrom.lock()->inventory->removeItems(it->first, it->second);
			this->getParent()->inventory->addItems(it->first, it->second);
		}
		this->currentAction = Action::DropingOff;
	}
	else if (this->currentAction == Action::DropingOff) {
		auto inventory = this->getParent()->inventory->getInventory();
		for (auto it = inventory.begin(); it != inventory.end(); it++) {
			this->getParent()->inventory->removeItems(it->first, it->second);
			this->giveTo.lock()->inventory->addItems(it->first, it->second);
		}
		this->currentAction = Action::PickingUp;
	}
}