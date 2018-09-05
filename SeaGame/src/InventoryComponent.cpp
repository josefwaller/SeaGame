#include "InventoryComponent.h"
#include "RenderComponent.h"
#include "Box2dTransform.h"

InventoryComponent::InventoryComponent(std::shared_ptr<Entity> parent) : Component(parent) {
}
void InventoryComponent::addItems(GameResource res, unsigned int amount) {
	this->inventory[res] += amount;
	if (this->getParent()->gui != nullptr) {
		this->getParent()->gui->updateInventory();
	}
}
void InventoryComponent::removeItems(GameResource res, unsigned int amount) {
	this->inventory[res] -= amount;
	if (this->getParent()->gui != nullptr) {
		this->getParent()->gui->updateInventory();
	}
}
std::map<GameResource, unsigned int> InventoryComponent::getInventory() {
	return this->inventory;
}