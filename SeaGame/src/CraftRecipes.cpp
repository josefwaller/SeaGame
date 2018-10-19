#include "CraftRecipes.h"
#include "EntityPrefabs.h"

std::vector<CraftingRecipes::CraftRecipe> CraftingRecipes::recipes = {
	{
		{
			{ GameResource::Gold, Wood }
		},
		Technology::Nothing,
		"Mining Base",
		[&](Game* g, sf::Vector2f pos) {
			g->addEntity(EntityPrefabs::miningBase(g, (sf::Vector2i)(pos / 64.0f) * 64));
			return true;
		}
	},
	{
		{},
		Technology::MakingStuff,
		"Military Base",
		[&](Game* g, sf::Vector2f pos) {
			g->addEntity(EntityPrefabs::militaryBase(g, (sf::Vector2i)(pos / 64.0f) * 64));
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
			g->addEntity(EntityPrefabs::city(g, (sf::Vector2i)(pos / 64.0f) * 64));
			return true;
		}
	},
	{
		{},
		Technology::Nothing,
		"Forestry",
		[&](Game* g, sf::Vector2f pos) {
			g->addEntity(EntityPrefabs::forestryBase(g, (sf::Vector2i)(pos / 64.0f) * 64));
			return true;
		}
	},
	{
		{},
		Technology::Nothing,
		"WoodCutters",
		[&](Game* g, sf::Vector2f pos) {
			g->addEntity(EntityPrefabs::conversionBase(g, (sf::Vector2i)(pos / 64.0f) * 64, GameResource::Planks));
			return true;
		}
	}
};
#include "Game.h"
