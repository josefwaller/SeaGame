#include "ConversionBaseController.h"
#include "InventoryComponent.h"
#include "ConversionRecipes.h"
#include "SaveData.h"

ConversionBaseController::ConversionBaseController(GameResource res) {
	this->product = res;
}
void ConversionBaseController::setParent(std::weak_ptr<Entity> parent) {
	BaseController::setParent(parent);
#ifdef _DEBUG
	if (this->getComponentList().inventory == nullptr) {
	}
	if (ConversionRecipes::recipes.find(this->product) == ConversionRecipes::recipes.end()) {
	}
#endif
}

void ConversionBaseController::update(float delta) {
	this->sinceLastConversion += delta;
	ConversionRecipes::Recipe target = ConversionRecipes::recipes.find(this->product)->second;
	// Check the base has the resources
	bool hasResources = true;
	auto inv = this->getComponentList().inventory->getInventory();
	for (auto it : target.cost) {
		if (inv[it.first] < it.second) {
			// Restart clock
			this->sinceLastConversion = 0.0f;
			hasResources = false;
			break;
		}
	}
	if (hasResources) {
		// check enough time has gone by
		if (this->sinceLastConversion >= target.duration) {
			// Remove items from inventory
			for (auto it : target.cost) {
				this->getComponentList().inventory->removeItems(it.first, it.second);
			}
			// Add a new product
			this->getComponentList().inventory->addItems(this->product, 1);
			// Alert player
			this->getGame()->getHud()->addAnnouncement("Added a " + getResourceString(this->product));
			// Restart clock
			this->sinceLastConversion = 0.0f;
		}
	}
}

SaveData ConversionBaseController::getSaveData() {
	return SaveData("Component", {
		{ "product", std::to_string((unsigned int)this->product) },
		{ "sinceLastConversion", std::to_string(this->sinceLastConversion) },
	});
}

void ConversionBaseController::fromSaveData(SaveData data) {
	this->product = (GameResource)std::stoi(data.getValue("product"));
	this->sinceLastConversion = std::stof(data.getValue("sinceLastConversion"));
}

GameResource ConversionBaseController::getProduct() {
	return this->product;
}
ConversionRecipes::Recipe ConversionBaseController::getRecipe() {
	return ConversionRecipes::recipes.find(this->product)->second;
}