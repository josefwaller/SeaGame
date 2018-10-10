#pragma once
#include "CityController.h"
#include "InventoryComponent.h"

CityController::CityController(std::weak_ptr<Entity> parent) : BaseController(parent) {

}

void CityController::update(float delta) {
	// ToDo: Make sure citycontroller always has inventory on debug
	for (auto it : this->getParent().lock()->components.inventory->getInventory()) {
		// Don't sell gold, and make sure the city has at least one of the resource
		if (it.first != GameResource::Gold && it.second > 0) {
			this->getParent().lock()->components.inventory->removeItems(it.first, it.second);
			this->getParent().lock()->components.inventory->addItems(GameResource::Gold, 100);
		}
	}
}