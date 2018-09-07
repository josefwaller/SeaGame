#include "CraftRecipes.h"
#include "EntityPrefabs.h"
std::vector<CraftingRecipes::CraftRecipe> CraftingRecipes::recipes = {
	{
		{},
		"Mining Base",
		[&](Game* g, sf::Vector2f pos) {
			g->addEntity(EntityPrefabs::miningBase(g, (sf::Vector2i)(pos / 64.0f)));
			return true;
		}
	},
	{
		{},
		"Military Base",
		[&](Game* g, sf::Vector2f pos) {
			g->addEntity(EntityPrefabs::militaryBase(g, (sf::Vector2i)(pos / 64.0f)));
			return true;
		}
	},
	{
		{},
		"Ferry Ship",
		[&](Game* g, sf::Vector2f pos) {
			g->addEntity(EntityPrefabs::ferryShip(g, pos, {}, {}));
			return true;
		}
	}
};
#include "Game.h"
