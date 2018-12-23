#pragma once
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <memory>
class Game;
struct Entity;
#include "GameResource.h"
#include "SFML/Graphics.hpp"
#include "TechTree.h"

// Recipes for the player building an entity in the world
// NOT for building resources from other resources, those
// are called conversion recipes
class CraftingRecipes {
public:
	struct CraftRecipe {
		// Resources required to build this entity
		std::map<GameResource, unsigned int> requiredResources;
		// The technology needed to build this item, or Nothing if nothing is needed
		Technology requiredTech;
		// Text to display on the button
		std::string displayText;
		// Method to build the entity
		// Returns the entity, may be invalid because of position or other reason, will check before building:w
		std::function<std::shared_ptr<Entity>(Game* game, sf::Vector2f pos)> createMethod;
	};
	static std::vector<CraftRecipe> recipes;
};