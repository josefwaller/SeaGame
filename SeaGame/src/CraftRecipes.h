#pragma once
#include <string>
#include <map>
class Game;
#include "GameResource.h"
#include "EntityPrefabs.h"

class CraftingRecipes {
public:
	struct CraftRecipe {
	public:
		// Resources required to build this entity
		std::map<GameResource, int> requiredResources;
		// Text to display on the button
		std::string displayText;
		// Method to build the entity
		// Returns true if successful, false otherwise
		std::function<bool(Game* game, sf::Vector2f pos)> createMethod;
	};
	static std::vector<CraftRecipe> recipes;
};
