#include "ConversionRecipes.h"

const std::map<GameResource, ConversionRecipes::Recipe> ConversionRecipes::recipes = {
	{ GameResource::Plank, {
		// What is needed
		{ { GameResource::Wood, 5 } },
		// How long it takes
		5.0f,
		// Text to display
		"Cut Wood into Plank",
		"Woodcutter"
		}
	},
	{ GameResource::Furniture, {
		{ {GameResource::Plank, 5 } },
		5.0f,
		"Build furniture from planks"
		}
	},
	{ GameResource::StoneBrick, {
		{ { GameResource::Stone, 3 }, },
		10.0f,
		"Cut Stone into Bricks",
		"Stonecutter"
		}
	},
	{ GameResource::StoneStatue, {
		{ { GameResource::Stone, 5}, },
		20.0f,
		"Make a sonte statue",
		"Statue Maker"
		}
	},
	// Grind wheat into flour
	{ GameResource::Flour, {
		{ {GameResource::Wheat, 2} },
		2.0f,
		"Grind wheat into flour",
		"Mill"
		}
	},
	// Bake flour into bread
	{ GameResource::Bread, {
		{ {GameResource::Flour, 5}},
		5.0f,
		"Bake flour into bread",
		"Bakery"
		}
	},
	{GameResource::Beer, {
		{ { GameResource::Fruit, 20}},
		20.0f,
		"Brew Wheat into Beer",
		"Brewery"
		}
	},
	{ GameResource::Steel, {
		{ {GameResource::Iron, 2} },
		5.0f,
		"Smelt Iron into Steel",
		"Smith"
		}
	},
	{ GameResource::Weapons, {
		{ {GameResource::Steel, 1 } },
		5.0f,
		"Forge Steel into weapons",
		"Armory"
		}
	}
};