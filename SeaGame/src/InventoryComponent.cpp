#include "InventoryComponent.h"
#include "RenderComponent.h"
#include "Box2dTransform.h"

InventoryComponent::InventoryComponent(std::shared_ptr<Entity> parent) : Component(parent) {
}
void InventoryComponent::addItems(GameResource res, unsigned int amount) {
	this->inventory[res] += amount;
	if (this->getParent().lock()->components.gui != nullptr) {
		this->getParent().lock()->components.gui->updateInventory();
	}
}
void InventoryComponent::removeItems(GameResource res, unsigned int amount) {
	this->inventory[res] -= amount;
	if (this->getParent().lock()->components.gui != nullptr) {
		this->getParent().lock()->components.gui->updateInventory();
	}
}
std::map<GameResource, unsigned int> InventoryComponent::getInventory() {
	return this->inventory;
}

std::map<std::string, std::string> InventoryComponent::getSaveData() {
	std::map<std::string, std::string> data;
	for (auto it = this->inventory.begin(); it != this->inventory.end(); ++it) {
		data.insert({ std::to_string(it->first), std::to_string(it->second) });
	}
	return data;
}