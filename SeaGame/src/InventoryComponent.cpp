#include "InventoryComponent.h"
#include "RenderComponent.h"
#include "Box2dTransform.h"

InventoryComponent::InventoryComponent(std::shared_ptr<Entity> parent) : Component(parent) {
	this->addItems(GameResource::Wood, 500);
}
void InventoryComponent::addItems(GameResource res, unsigned int amount) {
	this->inventory[res] += amount;
	if (this->getParent().lock()->components.gui != nullptr) {
		this->getParent().lock()->components.gui->update();
	}
}
void InventoryComponent::removeItems(GameResource res, unsigned int amount) {
	this->inventory[res] -= amount;
	if (this->getParent().lock()->components.gui != nullptr) {
		this->getParent().lock()->components.gui->update();
	}
}
std::map<GameResource, unsigned int> InventoryComponent::getInventory() {
	return this->inventory;
}

std::map<std::string, std::string> InventoryComponent::getSaveData() {
	std::map<std::string, std::string> data;
	for (auto it = this->inventory.begin(); it != this->inventory.end(); ++it) {
		data.insert({ "inv-" + std::to_string(it->first), std::to_string(it->second) });
	}
	return data;
}

void InventoryComponent::fromSaveData(std::map<std::string, std::string> data) {
	for (auto it = data.begin(); it != data.end(); ++it) {
		// Check the string begins with inv-
		if (it->first.find("inv-") == 0) {
			// Get resource type and number of resource
			int resNum = std::stoi(it->first.substr(4, it->first.size()));
			int resCount = std::stoi(it->second);
			// Add it
			this->addItems((GameResource)resNum, resCount);
		}
	}
}


void InventoryComponent::updateGui(tgui::Tabs::Ptr tabs, std::map<std::string, tgui::Panel::Ptr>* panels) {
	tabs->add("Inventory", false);
	panels->insert({ "Inventory", tgui::Panel::create() });
	panels->at("Inventory")->setPosition({ 0, tabs->getFullSize().y });
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
			this->getParent().lock()->game,
			this->getParent(),
			it.first,
			it.second);
		panels->at("Inventory")->add(btn);
		y += btn->getFullSize().y;
	}
	auto x = 0;
}
