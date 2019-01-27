#include "CraftRecipes.h"
#include "EntityPrefabs.h"

sf::Vector2f getBaseCoords(sf::Vector2f c) {
	return sf::Vector2f(sf::Vector2i(c / 64.0f) * 64);
}

std::vector<CraftingRecipes::CraftRecipe> CraftingRecipes::recipes = {
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
			return EntityPrefabs::generationBase(g, getBaseCoords(pos), GameResource::Stone);
		}
	},
	{
		{},
		Technology::IronMining,
		"Iron Mine",
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::generationBase(g, getBaseCoords(pos), GameResource::Iron);
		}
	},
	{
		{},
		Technology::CopperMining,
		"Copper Mine",
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::generationBase(g, getBaseCoords(pos), GameResource::Copper);
		}
	},
	{
		{},
		Technology::GoldMining,
		"Gold Mine",
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::generationBase(g, getBaseCoords(pos), GameResource::Gold);
		}
	},
	{
		{},
		Technology::Farming,
		"Farm",
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::generationBase(g, getBaseCoords(pos), GameResource::Wheat);
		}
	},
	{
		{},
		Technology::Mills,
		"Mill",
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::conversionBase(g, getBaseCoords(pos), GameResource::Flour);
		}
	},
	{
		{},
		Technology::Baking,
		"Bakery",
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::conversionBase(g, getBaseCoords(pos), GameResource::Bread);
		}
	},
	{
		{},
		Technology::Orchards,
		"Orchard",
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::generationBase(g, getBaseCoords(pos), GameResource::Fruit);
		}
	},
	{
		{},
		Technology::Breweries,
		"Brewery",
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::conversionBase(g, getBaseCoords(pos), GameResource::Beer);
		}
	},
	{
		{},
		Technology::Smelting,
		"Furnace",
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::conversionBase(g, getBaseCoords(pos), GameResource::Steel);
		}
	},
	{
		{},
		Technology::Forgery,
		"Forge",
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::conversionBase(g, getBaseCoords(pos), GameResource::Weapons);
		}
	}
};
#include "Game.h"
