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
		this->destination = this->stops[this->currentStopIndex];
		this->setTarget(this->getCoordsForEntity(this->destination));
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
	auto destInv = this->destination.lock()->components.inventory;
	auto thisInv = this->getParent().lock()->components.inventory;
	// Get the destination's inventory
	auto destInvData = destInv->getInventory();
	// Get this ship's inventory
	auto thisInvData = thisInv->getInventory();
	// Swap inventories
	for (auto it : destInvData) {
		destInv->removeItems(it.first, it.second);
		thisInv->addItems(it.first, it.second);
	}
	for (auto it : thisInvData) {
		destInv->addItems(it.first, it.second);
		thisInv->removeItems(it.first, it.second);
	}
	// Go to next target
	this->currentStopIndex = (this->currentStopIndex + 1) % this->stops.size();
	this->destination = this->stops[this->currentStopIndex];
	this->setTarget(this->getCoordsForEntity(this->destination));
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
		toReturn["stop_" + std::to_string(std::distance(this->stops.begin(), it))] = std::to_string(it->lock()->id);
	}
	return toReturn;
}

void FerryShipController::fromSaveData(std::map<std::string, std::string> data) {
	size_t count = std::stoi(data["stopCount"]);
	for (size_t i = 0; i < count; i++) {
		unsigned int id = std::stoi(data["stop_" + std::to_string(i)]);
		this->stops.push_back(this->getParent().lock()->game->getEntityById(id));
	}
}