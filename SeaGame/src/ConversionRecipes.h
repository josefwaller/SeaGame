#pragma once
#include "GameResource.h"
#include <map>

class ConversionRecipes {
public:
	struct Recipe {
		// How much of what resource is needed
		std::map<GameResource, unsigned int> cost;
		// The time take to build it, in seconds
		float duration;
		// The text used for UI
		std::string recipeText;
		// The name of the base that uses this recipe
		std::string baseName;
	};
	// The recipes, ordered by what they make
	static const std::map<GameResource, Recipe> recipes;
};