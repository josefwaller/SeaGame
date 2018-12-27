#include "CraftRecipes.h"
#include "EntityPrefabs.h"

std::vector<CraftingRecipes::CraftRecipe> CraftingRecipes::recipes = {
	// TODO: Remove this and make cities generated automatically on the map
	{
		{},
		Technology::Nothing,
		"City",
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::city(g, (sf::Vector2i)(pos / 64.0f) * 64);
		}
	},
	{
		{
			{ GameResource::Wood, 5 }
		},
		Technology::Ferries,
		"Ferry",
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::ferryShip(g, pos, {}, {});
		}
	},
	{
		{},
		Technology::Mines,
		"Mine",
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::generationBase(g, sf::Vector2i(pos / 64.0f) * 64, GameResource::Stone);
		}
	},
	{
		{},
		Technology::IronMining,
		"Iron Mine",
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::generationBase(g, sf::Vector2i(pos / 64.0f) * 64, GameResource::Iron);
		}
	},
	{
		{},
		Technology::CopperMining,
		"Copper Mine",
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::generationBase(g, sf::Vector2i(pos / 64.0f) * 64, GameResource::Copper);
		}
	},
	{
		{},
		Technology::GoldMining,
		"Gold Mine",
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::generationBase(g, sf::Vector2i(pos / 64.0f) * 64, GameResource::Gold);
		}
	},
	{
		{},
		Technology::Farming,
		"Farm",
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::generationBase(g, sf::Vector2i(pos / 64.0f) * 64, GameResource::Wheat);
		}
	},
	{
		{},
		Technology::Mills,
		"Mill",
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::conversionBase(g, sf::Vector2i(pos / 64.0f) * 64, GameResource::Flour);
		}
	},
	{
		{},
		Technology::Baking,
		"Bakery",
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::conversionBase(g, sf::Vector2i(pos / 64.0f) * 64, GameResource::Bread);
		}
}
};
#include "Game.h"
