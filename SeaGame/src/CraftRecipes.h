#pragma once
#include <string>
#include <map>
#include <vector>
#include <functional>
class Game;
#include "GameResource.h"
#include "SFML/Graphics.hpp"
#include "TechTree.h"

class CraftingRecipes {
public:
	struct CraftRecipe {
		// Resources required to build this entity
		std::map<GameResource, int> requiredResources;
		// The technology needed to build this item, or Nothing if nothing is needed
		Technology requiredTech;
		// Text to display on the button
		std::string displayText;
		// Method to build the entity
		// Returns true if successful, false otherwise
		std::function<bool(Game* game, sf::Vector2f pos)> createMethod;
	};
	static std::vector<CraftRecipe> recipes;
};
