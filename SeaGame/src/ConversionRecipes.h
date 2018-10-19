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
	};
	// The recipes, ordered by what they make
	static std::map<GameResource, Recipe> recipes;
};