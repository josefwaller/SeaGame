#pragma once
#include "TGUI/TGUI.hpp"
#include "Component.h"
#include "GameResource.h"
/*
 * Hold the inventory of an entity
 */
class InventoryComponent : public Component {
public:
	InventoryComponent(std::shared_ptr<Entity> parent);
	// Open the menu
	void openMenu();
	// Add/remove items to/from the inventory
	void addItems(GameResource res, unsigned int amount);
	void removeItems(GameResource res, unsigned int amount);
	// Check if the entity was clicked, and the menu should be opened
	void checkForOpen();
	std::string getResourceString(GameResource res);
private:
	// Inventory data
	std::map<GameResource, unsigned int> inventory;
	// GUI widgets
	tgui::ChildWindow::Ptr inventoryWindow;
	tgui::ListBox::Ptr inventoryText;
	// Set up the items of the widget
	void resetItems();
};