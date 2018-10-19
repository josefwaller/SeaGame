#include "ConversionRecipes.h"

std::map<GameResource, ConversionRecipes::Recipe> recipes = {
	{ GameResource::Planks, {
		// What is needed
		{ { GameResource::Wood, 5 } },
		// How long it takes
		5.0f,
		// Text to display
		"Cut Wood into Planks"
		}
	},
	{ GameResource::StoneBricks, {
		{ { GameResource::Stone, 3 }, },
		10.0f,
		"Cut Stone into Bricks"
		}
	}
};