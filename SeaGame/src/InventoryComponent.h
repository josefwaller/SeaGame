#pragma once
#include "TGUI/TGUI.hpp"
#include "Component.h"
#include "GameResource.h"
/*
 * Hold the inventory of an entity
 */
class InventoryComponent : public Component {
public:
	InventoryComponent();
	// Add/remove items to/from the inventory
	void addItems(GameResource res, unsigned int amount);
	void removeItems(GameResource res, unsigned int amount);
	// Retrieve a copy of the inventory's data
	std::map<GameResource, unsigned int> getInventory();
	SaveData getSaveData() override;
	void fromSaveData(SaveData data) override;
	virtual tgui::Widget::Ptr getGui() override;
private:
	// Inventory data
	std::map<GameResource, unsigned int> inventory;
	// Gui container
	tgui::ScrollablePanel::Ptr panel;
	// Reset the GUI in the panel
	void updatePanel();
};