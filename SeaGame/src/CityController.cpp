#pragma once
#include "CityController.h"
#include "InventoryComponent.h"

CityController::CityController(std::weak_ptr<Entity> parent) : BaseController(parent) {

}

void CityController::update(float delta) {
	// ToDo: Make sure citycontroller always has inventory on debug
	for (auto it : this->getParent().lock()->components.inventory->getInventory()) {
		// Check the city has at least one of this resource
		if (it.second > 0) {
			// Sell each one for $10 currently
			// TBA: different thigns sell differently
			this->getParent().lock()->game->addMoney(10 * it.second);
			this->getParent().lock()->components.inventory->removeItems(it.first, it.second);
		}
	}
}