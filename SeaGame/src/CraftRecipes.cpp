#include "CraftRecipes.h"
#include "EntityPrefabs.h"

std::vector<CraftingRecipes::CraftRecipe> CraftingRecipes::recipes = {
	{
		{
			{ GameResource::Gold, 50 }
		},
		Technology::Nothing,
		"Mining Base",
		[&](Game* g, sf::Vector2f pos) {
			g->addEntity(EntityPrefabs::miningBase(g, (sf::Vector2i)(pos / 64.0f)));
			return true;
		}
	},
	{
		{},
		Technology::MakingStuff,
		"Military Base",
		[&](Game* g, sf::Vector2f pos) {
			g->addEntity(EntityPrefabs::militaryBase(g, (sf::Vector2i)(pos / 64.0f)));
			return true;
		}
	},
	{
		{},
		Technology::MakingStuff2,
		"Ferry Ship",
		[&](Game* g, sf::Vector2f pos) {
			g->addEntity(EntityPrefabs::ferryShip(g, pos, {}, {}));
			return true;
		}
	},
	{
		{},
		Technology::Nothing,
		"City",
		[&](Game* g, sf::Vector2f pos) {
			g->addEntity(EntityPrefabs::city(g, (sf::Vector2i)(pos / 64.0f)));
			return true;
		}
	}
};
#include "Game.h"
