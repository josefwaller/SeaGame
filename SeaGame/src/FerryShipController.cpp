#include "FerryShipController.h"
#include "BaseController.h"
#include "InventoryComponent.h"

FerryShipController::FerryShipController(std::weak_ptr<Entity> parent): AutomatedShipController(parent) {
}
void FerryShipController::update(float delta) {
	// Move towards destination if it exists
	if (this->destination.lock()) {
		this->move(delta);
	}
	else if (this->stops.size() > 0) {
		// Skip this stop
		// Todo: add a warnign to the player here?
		this->currentStopIndex = (this->currentStopIndex + 1) % this->stops.size();
		this->destination = this->stops[this->currentStopIndex].target;
		this->setTarget(this->getCoordsForEntity(this->destination));
	}
}
std::vector<FerryShipController::FerryStop> FerryShipController::getStops() {
	return this->stops;
}
void FerryShipController::addStop(std::weak_ptr<Entity> stop) {
	this->stops.push_back({
		stop,
		{},
		{}
	});
}
// Move the stop at currentOrder to newOrder and shift the other stops down by one
void FerryShipController::setStopOrder(size_t currentOrder, size_t newOrder) {
	// Copy
	FerryStop stop = this->stops[currentOrder];
	// Remove
	this->stops.erase(this->stops.begin() + currentOrder);
	// Reinsert
	this->stops.insert(this->stops.begin() + newOrder, stop);
}
void FerryShipController::onReachingTarget() {
	auto destInv = this->destination.lock()->components.inventory;
	auto thisInv = this->getParent().lock()->components.inventory;
	// Get the destination's inventory
	auto destInvData = destInv->getInventory();
	// Get this ship's inventory
	auto thisInvData = thisInv->getInventory();
	// Take things from the  destination
	for (auto it : this->stops[currentStopIndex].toPickUp) {
		// IF the ship should pick up this resource
		if (it.second) {
			destInv->removeItems(it.first, destInvData[it.first]);
			thisInv->addItems(it.first, destInvData[it.first]);
		}
	}
	for (auto it : this->stops[currentStopIndex].toDropOff) {
		if (it.second) {
			destInv->addItems(it.first, thisInvData[it.first]);
			thisInv->removeItems(it.first, thisInvData[it.first]);
		}
	}
	// Go to next target
	this->currentStopIndex = (this->currentStopIndex + 1) % this->stops.size();
	this->destination = this->stops[this->currentStopIndex].target;
	this->setTarget(this->getCoordsForEntity(this->destination));
}
void FerryShipController::setStopPickUp(size_t stopIndex, GameResource res, bool val) {
	this->stops[stopIndex].toPickUp[res] = val;
}
void FerryShipController::setStopDropOff(size_t stopIndex, GameResource res, bool val) {
	this->stops[stopIndex].toDropOff[res] = val;
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
	std::string stopCount = std::to_string(this->stops.size());
	toReturn["stopCount"] = stopCount;
	for (auto it = this->stops.begin(); it != this->stops.end(); it++) {
		toReturn["stop_" + std::to_string(std::distance(this->stops.begin(), it))] = std::to_string(it->target.lock()->id);
	}
	return toReturn;
}

void FerryShipController::fromSaveData(std::map<std::string, std::string> data) {
	size_t count = std::stoi(data["stopCount"]);
	for (size_t i = 0; i < count; i++) {
		unsigned int id = std::stoi(data["stop_" + std::to_string(i)]);
		this->stops.push_back({
			this->getParent().lock()->game->getEntityById(id),
			{},
			{}
		});
	}
	this->getParent().lock()->components.gui->update();
}