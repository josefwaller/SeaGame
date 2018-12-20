#pragma once
#include "CityController.h"
#include "InventoryComponent.h"
#include "GameResource.h"

void CityController::update(float delta) {
	// ToDo: Make sure citycontroller always has inventory on debug
	for (auto it : this->getParent().lock()->components.inventory->getInventory()) {
		// Check the city has at least one of this resource
		if (it.second > 0) {
			// Sell each one for $10 currently
			this->getParent().lock()->game->addMoney(RESOURCE_VALUES.find(it.first)->second * it.second);
			this->getParent().lock()->components.inventory->removeItems(it.first, it.second);
		}
	}
}