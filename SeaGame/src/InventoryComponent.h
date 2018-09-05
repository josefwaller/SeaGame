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
	// Add/remove items to/from the inventory
	void addItems(GameResource res, unsigned int amount);
	void removeItems(GameResource res, unsigned int amount);
	std::map<GameResource, unsigned int> getInventory();
private:
	// Inventory data
	std::map<GameResource, unsigned int> inventory;
};