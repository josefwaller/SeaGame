#include "FerryShipController.h"
#include "BaseController.h"
#include "InventoryComponent.h"

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
std::vector<std::weak_ptr<Entity>> FerryShipController::getStops() {
	return this->stops;
}
void FerryShipController::addStop(std::weak_ptr<Entity> stop) {
	this->stops.push_back(stop);
}
// Move the stop at currentOrder to newOrder and shift the other stops down by one
void FerryShipController::setStopOrder(size_t currentOrder, size_t newOrder) {
	// Copy
	std::weak_ptr<Entity> stop = this->stops[currentOrder];
	// Remove
	this->stops.erase(this->stops.begin() + currentOrder);
	// Reinsert
	this->stops.insert(this->stops.begin() + newOrder, stop);
}
void FerryShipController::onReachingTarget() {
	if (this->currentAction == Action::PickingUp && this->source.lock()) {
		// Take all of the base's things
		auto inventory = this->source.lock()->components.inventory->getInventory();
		for (auto it = inventory.begin(); it != inventory.end(); it++) {
			this->source.lock()->components.inventory->removeItems(it->first, it->second);
			this->getParent().lock()->components.inventory->addItems(it->first, it->second);
		}
		// Go to the destination
		this->currentAction = Action::DropingOff;
		this->setTarget(this->getCoordsForEntity(this->destination));
	}
	else if (this->currentAction == Action::DropingOff && this->destination.lock()) {
		auto inventory = this->getParent().lock()->components.inventory->getInventory();
		for (auto it = inventory.begin(); it != inventory.end(); it++) {
			this->getParent().lock()->components.inventory->removeItems(it->first, it->second);
			this->destination.lock()->components.inventory->addItems(it->first, it->second);
		}
		// Go to the source
		this->currentAction = Action::PickingUp;
		this->setTarget(this->getCoordsForEntity(this->source));
	}
}
sf::Vector2f FerryShipController::getCoordsForEntity(std::weak_ptr<Entity> e) {
	if (auto b = std::dynamic_pointer_cast<BaseController>(e.lock()->components.controller)) {
		return b->getDockCoords();
	}
	else {
		return e.lock()->components.transform->getPosition();
	}
}

std::map<std::string, std::string> FerryShipController::getSaveData() {
	std::map<std::string, std::string> toReturn;
	if (this->source.lock()) {
		toReturn["source"] = std::to_string(this->source.lock()->id);
	}
	if (this->destination.lock()) {
		toReturn["destination"] = std::to_string(this->destination.lock()->id);
	}
	return toReturn;
}

void FerryShipController::fromSaveData(std::map<std::string, std::string> data) {
	if (data.find("source") != data.end()) {
		this->setSource(this->getParent().lock()->game->getEntityById(std::stoi(data["source"])));
	}
	if (data.find("destination") != data.end()) {
		this->setDestination(this->getParent().lock()->game->getEntityById(std::stoi(data["destination"])));
	}
}