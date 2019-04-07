#pragma once
#include "CityController.h"
#include "InventoryComponent.h"
#include "GameResource.h"

void CityController::update(float delta) {
	// ToDo: Make sure citycontroller always has inventory on debug
	for (auto it : this->getComponentList().inventory->getInventory()) {
		// Check the city has at least one of this resource
		if (it.second > 0) {
			// Sell each one for $10 currently
			this->getGame()->addMoney(RESOURCE_VALUES.find(it.first)->second * it.second);
			this->getComponentList().inventory->removeItems(it.first, it.second);
		}
	}
}