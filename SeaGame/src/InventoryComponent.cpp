#include "InventoryComponent.h"
#include "RenderComponent.h"
#include "Box2dTransform.h"

InventoryComponent::InventoryComponent(std::shared_ptr<Entity> parent) : Component(parent) {
	this->inventoryWindow = tgui::ChildWindow::create();
	this->inventoryWindow->setTitle("Inventory");
	this->inventoryText = tgui::ListBox::create();
	this->inventoryWindow->add(this->inventoryText);
}
void InventoryComponent::addItems(GameResource res, unsigned int amount) {
	this->inventory[res] += amount;
	this->resetItems();
}
void InventoryComponent::removeItems(GameResource res, unsigned int amount) {
	this->inventory[res] -= amount;
	this->resetItems();
}
void InventoryComponent::openMenu() {
	this->getParent()->game->getGui().add(this->inventoryWindow);
}
void InventoryComponent::checkForOpen() {
	sf::Vector2f mouseCoords = this->getParent()->game->getMouseCoords();
	if (auto trans = std::dynamic_pointer_cast<Box2dTransform>(this->getParent()->transform)) {
		auto body = trans->getBody();
		for (auto fix = body->GetFixtureList(); fix; fix = fix->GetNext()) {
			if (fix->TestPoint(b2Vec2(mouseCoords.x, mouseCoords.y))) {
				this->openMenu();
				return;
			}
		}
	}
}
std::string InventoryComponent::getResourceString(GameResource res) {
	switch (res) {
	case GameResource::Gold: return "Gold";
	case GameResource::Wood: return "Wood";
	case GameResource::Stone: return "Stone";
	}
	return "N/A";
}
void InventoryComponent::resetItems() {
	this->inventoryText->deselectItem();
	this->inventoryText->removeAllItems();
	for (auto it = this->inventory.begin(); it != this->inventory.end(); it++) {
		std::string itemString = this->getResourceString(it->first) + std::string(": ") + std::to_string(it->second);
		this->inventoryText->addItem(itemString);
	}
}
std::map<GameResource, unsigned int> InventoryComponent::getInventory() {
	return this->inventory;
}