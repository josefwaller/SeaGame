#include "CraftRecipes.h"
#include "EntityPrefabs.h"
#include "ResourceManager.h"

// Name of sheet
const std::string SHEET = "medievalRTS_spritesheet@2";

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
		ResourceManager::get()->getSprite("ships", "sailLargeRed.png", false),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::ferryShip(g, pos, {}, {});
		}
	},
	{
		{},
		Technology::Mines,
		"Mine",
		getResourceSprite(GameResource::Stone),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::generationBase(g, getBaseCoords(pos), GameResource::Stone);
		}
	},
	{
		{},
		Technology::IronMining,
		"Iron Mine",
		getResourceSprite(GameResource::Iron),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::generationBase(g, getBaseCoords(pos), GameResource::Iron);
		}
	},
	{
		{},
		Technology::CopperMining,
		"Copper Mine",
		getResourceSprite(GameResource::Copper),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::generationBase(g, getBaseCoords(pos), GameResource::Copper);
		}
	},
	{
		{},
		Technology::GoldMining,
		"Gold Mine",
		getResourceSprite(GameResource::Gold),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::generationBase(g, getBaseCoords(pos), GameResource::Gold);
		}
	},
	{
		{},
		Technology::Farming,
		"Farm",
		getResourceSprite(GameResource::Wheat),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::generationBase(g, getBaseCoords(pos), GameResource::Wheat);
		}
	},
	{
		{},
		Technology::Mills,
		"Mill",
		getResourceSprite(GameResource::Flour),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::conversionBase(g, getBaseCoords(pos), GameResource::Flour);
		}
	},
	{
		{},
		Technology::Baking,
		"Bakery",
		getResourceSprite(GameResource::Bread),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::conversionBase(g, getBaseCoords(pos), GameResource::Bread);
		}
	},
	{
		{},
		Technology::Orchards,
		"Orchard",
		getResourceSprite(GameResource::Fruit),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::generationBase(g, getBaseCoords(pos), GameResource::Fruit);
		}
	},
	{
		{},
		Technology::Breweries,
		"Brewery",
		getResourceSprite(GameResource::Beer),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::conversionBase(g, getBaseCoords(pos), GameResource::Beer);
		}
	},
	{
		{},
		Technology::Smelting,
		"Furnace",
		getResourceSprite(GameResource::Steel),
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
