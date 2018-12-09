#include "CraftRecipes.h"
#include "EntityPrefabs.h"

std::vector<CraftingRecipes::CraftRecipe> CraftingRecipes::recipes = {
	// TODO: Remove this and make cities generated automatically on the map
	{
		{},
		Technology::Nothing,
		"City",
		[&](Game* g, sf::Vector2f pos) {
			g->addEntity(EntityPrefabs::city(g, (sf::Vector2i)(pos / 64.0f) * 64));
			return true;
		}
	},
	{
		{},
		Technology::Ferries,
		"Ferry",
		[&](Game* g, sf::Vector2f pos) {
			g->addEntity(EntityPrefabs::ferryShip(g, pos, {}, {}));
			return true;
		}
	},
	{
		{},
		Technology::Mines,
		"Mine",
		[&](Game* g, sf::Vector2f pos) {
			g->addEntity(EntityPrefabs::generationBase(g, sf::Vector2i(pos / 64.0f) * 64, GameResource::Stone));
			return true;
		}
	},
	{
		{},
		Technology::IronMining,
		"Iron Mine",
		[&](Game* g, sf::Vector2f pos) {
			g->addEntity(EntityPrefabs::generationBase(g, sf::Vector2i(pos / 64.0f) * 64, GameResource::Iron));
			return true;
		}
	},
	{
		{},
		Technology::CopperMining,
		"Copper Mine",
		[&](Game* g, sf::Vector2f pos) {
			g->addEntity(EntityPrefabs::generationBase(g, sf::Vector2i(pos / 64.0f) * 64, GameResource::Copper));
			return true;
		}
	},
	{
		{},
		Technology::GoldMining,
		"Gold Mine",
		[&](Game* g, sf::Vector2f pos) {
			g->addEntity(EntityPrefabs::generationBase(g, sf::Vector2i(pos / 64.0f) * 64, GameResource::Gold));
			return true;
		}
	}
};
#include "Game.h"
