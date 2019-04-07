#include "InventoryComponent.h"
#include "RenderComponent.h"
#include "Box2dTransform.h"
#include "SaveData.h"
#include "Entity.h"
#include <TGUI/Gui.hpp>

InventoryComponent::InventoryComponent() {
	this->panel = tgui::ScrollablePanel::create();
}
void InventoryComponent::addItems(GameResource res, unsigned int amount) {
	this->inventory[res] += amount;
	this->updatePanel();
}
void InventoryComponent::removeItems(GameResource res, unsigned int amount) {
	this->inventory[res] -= amount;
	this->updatePanel();
}
std::map<GameResource, unsigned int> InventoryComponent::getInventory() {
	return this->inventory;
}

SaveData InventoryComponent::getSaveData() {
	SaveData data("Component");
	for (auto it = this->inventory.begin(); it != this->inventory.end(); ++it) {
		SaveData invData("Inventory");
		invData.addValue("resource", std::to_string(it->first));
		invData.addValue("count", std::to_string(it->second));
		data.addData(invData);
	}
	return data;
}

void InventoryComponent::fromSaveData(SaveData data) {
	for (SaveData sd : data.getDatas()) {
		GameResource res = (GameResource)std::stoi(sd.getValue("resource"));
		unsigned int count = (unsigned int)std::stoi(sd.getValue("count"));
		this->addItems(res, count);
	}
}
tgui::Widget::Ptr InventoryComponent::getGui() {
	return this->panel;
}
void InventoryComponent::updatePanel() {
	this->panel->removeAllWidgets();
	float y = 0.0f;
	// Add a button for each of the resources
	for (auto it : this->getInventory()) {
		tgui::Button::Ptr btn = tgui::Button::create();
		btn->setText(getResourceString(it.first) + ": " + std::to_string(it.second));
		btn->setPosition({ 0.0f, y });
		// Make it transfer resources when clicked
		btn->connect("clicked",
			[&](Game* g, std::weak_ptr<Entity> e, GameResource res, unsigned int amount) {
				e.lock()->game->getHud()->transferItems(e, res, amount);
			},
			this->getGame(),
			this->getParent(),
			it.first,
			it.second);
		this->panel->add(btn);
		y += btn->getFullSize().y;
	}
}
