#include "GuiComponent.h"
#include "Box2dTransform.h"
#include "FerryShipController.h"
#include "MiningBaseController.h"

GuiComponent::GuiComponent(std::weak_ptr<Entity> parent) : Component(parent) {
	this->entityWindow = tgui::ChildWindow::create();
	this->entityTabs = tgui::Tabs::create();
	this->entityTabs->add("Inventory");
	this->entityTabs->add("Health");
	this->entityPanels["Inventory"] = tgui::Panel::create();
	this->entityPanels["Health"] = tgui::Panel::create();
	this->entityPanels["Inventory"]->setPosition({ 0, this->entityTabs->getFullSize().y });
	this->updateInventory();
	if (auto controller = std::dynamic_pointer_cast<FerryShipController>(this->getParent().lock()->components.controller)) {

		this->entityPanels["Ferry"] = tgui::Panel::create();
		this->entityTabs->add("Ferry");
		auto destBtn = tgui::Button::create();
		destBtn->setText("Change Destination");
		destBtn->connect("clicked", [&](Game* game, std::weak_ptr<FerryShipController> cont) {
			// Function which sets the destination to the entity
			// Bind the FerryShipController to the callback, so as to properly set the destination
			auto callback = std::bind([&](std::weak_ptr<Entity> e, std::weak_ptr<FerryShipController> c) {
				if (c.lock() && e.lock()->tag == EntityTag::Base) {
					c.lock()->setDestination(e);
				}
			}, std::placeholders::_1, cont);
			// Set callback
			game->getHud()->selectEntity(callback);
		}, this->getParent().lock()->game, controller);
		this->entityPanels["Ferry"]->add(destBtn);
		// Add the Change Source button
		auto srcBtn = tgui::Button::create();
		srcBtn->setText("Change Source");
		srcBtn->connect("clicked", [&](Game * game, std::weak_ptr<FerryShipController> cont) {
			// Pretty much the same as above
			auto callback = std::bind([&](std::weak_ptr<Entity> e, std::weak_ptr<FerryShipController> c) {
				if (c.lock() && e.lock()->tag == EntityTag::Base) {
					c.lock()->setSource(e);
				}
			}, std::placeholders::_1, cont);
			game->getHud()->selectEntity(callback);
		}, this->getParent().lock()->game, controller);
		// Move below the set dest button
		srcBtn->setPosition({ 0, destBtn->getFullSize().y });
		this->entityPanels["Ferry"]->add(srcBtn);
	}
	this->entityTabs->select("Inventory");
	this->selectedPanel = "Inventory";
	this->entityTabs->connect("TabSelected", &GuiComponent::changePanel, this);
	this->entityWindow->add(this->entityTabs);
	this->entityWindow->add(this->entityPanels["Inventory"]);
}

void GuiComponent::changePanel(std::string selectedPanel) {
	this->entityWindow->remove(this->entityPanels[this->selectedPanel]);
	this->selectedPanel = selectedPanel;
	this->entityWindow->add(this->entityPanels[this->selectedPanel]);
	this->entityPanels[this->selectedPanel]->setPosition({ 0, this->entityTabs->getFullSize().y });
}
std::string GuiComponent::getResourceString(GameResource res) {
	switch (res) {
	case GameResource::Gold: return "Gold";
	case GameResource::Wood: return "Wood";
	case GameResource::Stone: return "Stone";
	case GameResource::Plank: return "Plank";
	case GameResource::StoneBrick: return "Stone Bricks";
	case GameResource::StoneStatue: return "Statue";
	}
	return "N/A";
}


void GuiComponent::updateInventory() {
	this->entityPanels["Inventory"]->removeAllWidgets();
	float y = 0.0f;
	// Add a button for each of the resources
	if (this->getParent().lock()->components.inventory != nullptr) {
		for (auto it : this->getParent().lock()->components.inventory->getInventory()) {
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
			this->entityPanels["Inventory"]->add(btn);
			y += btn->getFullSize().y;
		}
	}
}

void GuiComponent::onClick() {
	if (!this->entityWindow->getParent()) {
		this->getParent().lock()->game->getGui()->add(this->entityWindow);
	}
}
void GuiComponent::show() {
	this->getParent().lock()->game->getGui()->add(this->entityWindow);
}
void GuiComponent::hide() {
	this->getParent().lock()->game->getGui()->remove(this->entityWindow);
}