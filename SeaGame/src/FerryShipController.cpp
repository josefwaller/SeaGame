#include "FerryShipController.h"
#include "BaseController.h"

FerryShipController::FerryShipController(std::weak_ptr<Entity> parent, std::weak_ptr<Entity> from, std::weak_ptr<Entity> to)
	: AutomatedShipController(parent) {
	this->setDestination(to);
	this->setSource(from);
}
void FerryShipController::update(float delta) {
	if (this->source.lock() && this->destination.lock()) {
		this->move(delta);
	}
}
void FerryShipController::setDestination(std::weak_ptr<Entity> dest) {
	this->destination = dest;
}
void FerryShipController::setSource(std::weak_ptr<Entity> src) {
	this->source = src;
	if (this->source.lock()) {
		this->currentAction = Action::PickingUp;
		this->setTarget(this->getCoordsForEntity(this->source));
	}
}
void FerryShipController::onReachingTarget() {
	if (this->currentAction == Action::PickingUp && this->source.lock()) {
		// Take all of the base's things
		auto inventory = this->source.lock()->inventory->getInventory();
		for (auto it = inventory.begin(); it != inventory.end(); it++) {
			this->source.lock()->inventory->removeItems(it->first, it->second);
			this->getParent().lock()->inventory->addItems(it->first, it->second);
		}
		// Go to the destination
		this->currentAction = Action::DropingOff;
		this->setTarget(this->getCoordsForEntity(this->destination));
	}
	else if (this->currentAction == Action::DropingOff && this->destination.lock()) {
		auto inventory = this->getParent().lock()->inventory->getInventory();
		for (auto it = inventory.begin(); it != inventory.end(); it++) {
			this->getParent().lock()->inventory->removeItems(it->first, it->second);
			this->destination.lock()->inventory->addItems(it->first, it->second);
		}
		// Go to the source
		this->currentAction = Action::PickingUp;
		this->setTarget(this->getCoordsForEntity(this->source));
	}
}
sf::Vector2f FerryShipController::getCoordsForEntity(std::weak_ptr<Entity> e) {
	if (auto b = std::dynamic_pointer_cast<BaseController>(e.lock()->controller)) {
		return b->getDockCoords();
	}
	else {
		return e.lock()->transform->getPosition();
	}
}