#include "ConversionRecipes.h"

const std::map<GameResource, ConversionRecipes::Recipe> ConversionRecipes::recipes = {
	{ GameResource::Plank, {
		// What is needed
		{ { GameResource::Wood, 5 } },
		// How long it takes
		5.0f,
		// Text to display
		"Cut Wood into Plank"
		}
	},
	{ GameResource::StoneBrick, {
		{ { GameResource::Stone, 3 }, },
		10.0f,
		"Cut Stone into Bricks"
		}
	},
	{ GameResource::StoneStatue, {
		{ { GameResource::Stone, 5}, },
		20.0f,
		"MAke a sonte statue"
		}
	}
};