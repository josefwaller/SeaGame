#include "CraftRecipes.h"
#include "EntityPrefabs.h"
#include "ResourceManager.h"
#include "ResourceController.h"
#include "Entity.h"
#include <memory>
#include "Game.h"

// Utilitary definition for checkIfValid functions
#define checkFunction const std::function<bool(Game* g, sf::Vector2f)>

// Name of sheet
const std::string SHEET = "medievalRTS_spritesheet@2";

// Method to check if the base's location is valid
checkFunction baseValid = [&](Game* g, sf::Vector2f pos) {
	// Ensure the base is on the land
	sf::Vector2i coords = sf::Vector2i(pos / 64.0f);
	for (size_t x = 0; x < 3; x++) {
		for (size_t y = 0; y < 3; y++) {
			if (g->getGameMap()->getTileAt(coords.x + x, coords.y + y) == GameMap::TileType::Sea) {
				return false;
			}
		}
	}
	return true;
};
// Method to check a generation base's location is correct
// Just checks that it is a valid base location and that the resource is there
checkFunction generationBaseValid(GameResource neededRes) {
	return [neededRes](Game* g, sf::Vector2f pos) {
		if (baseValid(g, pos)) {
			// Check the resource is present
			for (std::shared_ptr<Entity> other : g->getEntities()) {
				// Check the entity is a resource and is the correct resource
				if (auto otherCont = std::dynamic_pointer_cast<ResourceController>(other->components.controller)) {
					if (otherCont->getResource() == neededRes) {
						sf::Vector2i otherPos = sf::Vector2i(other->components.transform->getPosition()) / 64;
						sf::Vector2i coords = sf::Vector2i(pos / 64.0f);
						if (otherPos.x - coords.x < 3 && otherPos.x - coords.x >= 0) {
							if (otherPos.y - coords.y < 3 && otherPos.y - coords.y >= 0) {
								return true;
							}
						}
					}
				}
			}
		}
		return false;
	};
}

// Method to check if a ship's location is valid
const std::function<bool(Game*, sf::Vector2f)> shipValid = [&](Game* g, sf::Vector2f pos) {
	return true;
};

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
		},
		shipValid
	},
	{
		{},
		Technology::Mines,
		"Mine",
		getResourceSprite(GameResource::Stone, false),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::generationBase(g, getBaseCoords(pos), GameResource::Stone);
		},
		generationBaseValid(GameResource::Stone)
	},
	{
		{},
		Technology::IronMining,
		"Iron Mine",
		getResourceSprite(GameResource::Iron, false),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::generationBase(g, getBaseCoords(pos), GameResource::Iron);
		},
		generationBaseValid(GameResource::Iron)
	},
	{
		{},
		Technology::CopperMining,
		"Copper Mine",
		getResourceSprite(GameResource::Copper, false),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::generationBase(g, getBaseCoords(pos), GameResource::Copper);
		},
		generationBaseValid(GameResource::Copper)
	},
	{
		{},
		Technology::GoldMining,
		"Gold Mine",
		getResourceSprite(GameResource::Gold, false),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::generationBase(g, getBaseCoords(pos), GameResource::Gold);
		},
		generationBaseValid(GameResource::Gold)
	},
	{
		{},
		Technology::Farming,
		"Farm",
		getResourceSprite(GameResource::Wheat, false),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::generationBase(g, getBaseCoords(pos), GameResource::Wheat);
		},
		baseValid
	},
	{
		{},
		Technology::Mills,
		"Mill",
		getResourceSprite(GameResource::Flour, false),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::conversionBase(g, getBaseCoords(pos), GameResource::Flour);
		},
		baseValid
	},
	{
		{},
		Technology::Baking,
		"Bakery",
		getResourceSprite(GameResource::Bread, false),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::conversionBase(g, getBaseCoords(pos), GameResource::Bread);
		},
		baseValid
	},
	{
		{},
		Technology::Orchards,
		"Orchard",
		getResourceSprite(GameResource::Fruit, false),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::generationBase(g, getBaseCoords(pos), GameResource::Fruit);
		},
		baseValid
	},
	{
		{},
		Technology::Breweries,
		"Brewery",
		getResourceSprite(GameResource::Beer, false),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::conversionBase(g, getBaseCoords(pos), GameResource::Beer);
		},
		baseValid
	},
	{
		{},
		Technology::Smelting,
		"Furnace",
		getResourceSprite(GameResource::Steel, false),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::conversionBase(g, getBaseCoords(pos), GameResource::Steel);
		},
		baseValid
	},
	{
		{},
		Technology::Forgery,
		"Forge",
		getResourceSprite(GameResource::Weapons, false),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::conversionBase(g, getBaseCoords(pos), GameResource::Weapons);
		},
		baseValid
	},
	{
		{},
		Technology::MilitaryBases,
		"Military Base",
		ResourceManager::get()->getSprite("ships", "cannon.png", false),
		[&](Game* g, sf::Vector2f(pos)) {
			return EntityPrefabs::militaryBase(g, getBaseCoords(pos));
		},
		baseValid
	}
};
#include "Game.h"
