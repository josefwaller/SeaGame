#include "ConversionBaseController.h"
#include "ConversionRecipes.h"

ConversionBaseController::ConversionBaseController(std::weak_ptr<Entity> parent, GameResource res) : BaseController(parent) {
	this->product = res;
#ifdef _DEBUG
	if (this->getParent().lock()->components.inventory == nullptr) {
		auto x = 0;
	}
	if (ConversionRecipes::recipes.find(this->product) == ConversionRecipes::recipes.end()) {
		auto x = 0;
	}
#endif
}

void ConversionBaseController::update(float delta) {
	ConversionRecipes::Recipe target = ConversionRecipes::recipes.find(this->product)->second;
	// Check the base has the resources
	bool hasResources = true;
	auto inv = this->getParent().lock()->components.inventory->getInventory();
	for (auto it : target.cost) {
		if (inv[it.first] < it.second) {
			// Restart clock
			this->conversionClock.restart();
			hasResources = false;
			break;
		}
	}
	if (hasResources) {
		// check enough time has gone by
		if (this->conversionClock.getElapsedTime().asSeconds() >= target.duration) {
			// Remove items from inventory
			for (auto it : target.cost) {
				this->getParent().lock()->components.inventory->removeItems(it.first, it.second);
			}
			// Add a new product
			this->getParent().lock()->components.inventory->addItems(this->product, 1);
			// Restart clock
			this->conversionClock.restart();
		}
	}
}