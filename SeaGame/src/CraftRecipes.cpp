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

// Method to check if the base's location is on land
bool baseOnLand(Game* g, sf::Vector2f pos) {
	// Ensure the base is on the land
	GameMap* gM = g->getGameMap();
	sf::Vector2i c = sf::Vector2i(pos / 64.0f);
	for (size_t x = 0; x < 3; x++) {
		for (size_t y = 0; y < 3; y++) {
			if (gM->getTileAt(c.x + x, c.y + y) == GameMap::TileType::Sea) {
				return false;
			}
		}
	}
	return true;
}
// Check if a bases location is on land and has no other entities on it
// Used for conversion bases and generation bases which do not require a resource (i.e. farms)
bool basePositionIsEmpty(Game* g, sf::Vector2f pos) {
	GameMap* gM = g->getGameMap();
	sf::Vector2i c = sf::Vector2i(pos / 64.0f);
	for (size_t x = 0; x < 3; x++) {
		for (size_t y = 0; y < 3; y++) {
			if (gM->getTileIsFull(c.x + x, c.y + x)) {
				return false;
			}
		}
	}
	return true;
}
bool basePositionHasResource(Game* g, sf::Vector2f pos, GameResource res) {
	sf::Vector2i c = sf::Vector2i(pos / 64.0f);
	// Check the resource is present
	bool hasResource = false;
	for (size_t x = 0; x < 3; x++) {
		for (size_t y = 0; y < 3; y++) {
			if (g->getGameMap()->getTileIsFull(c.x + x, c.y + y)) {
				std::shared_ptr<Entity> e = g->getGameMap()->getTileEntity(c.x + x, c.y + y).lock();
				// Check the entity is a resource and is the correct resource
				if (auto otherCont = std::dynamic_pointer_cast<ResourceController>(e->components.controller)) {
					if (otherCont->getResource() == res) {
						hasResource = true;
					}
					else {
						// There is some other resource here that blocks us
						return false;
					}
				}
				else {
					// There is another entity (i.e. a base) here that blocks us
					return false;
					}

				}
			}
		}
	if (hasResource) {
		return true;
	}
	return false;
}
checkFunction noResBaseValid = [&](Game* g, sf::Vector2f pos) {
	return basePositionIsEmpty(g, pos) && baseOnLand(g, pos);
};
// Method to check a generation base's location is correct
// Just checks that it is a valid base location and that the resource is there
checkFunction resBaseValid(GameResource neededRes) {
	return [neededRes](Game* g, sf::Vector2f pos) {
		return baseOnLand(g, pos) && basePositionHasResource(g, pos, neededRes);
	};
}
void setIsFullForBase(Game* g, sf::Vector2f pos, std::weak_ptr<Entity> e) {
	sf::Vector2i c = sf::Vector2i(pos / 64.0f);
	for (size_t x = 0; x < 3; x++) {
		for (size_t y = 0; y < 3; y++) {
			g->getGameMap()->setTileEntity(c.x + x, c.y + y, e);
		}
	}
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
		resBaseValid(GameResource::Stone)
	},
	{
		{},
		Technology::IronMining,
		"Iron Mine",
		getResourceSprite(GameResource::Iron, false),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::generationBase(g, getBaseCoords(pos), GameResource::Iron);
		},
		resBaseValid(GameResource::Iron)
	},
	{
		{},
		Technology::CopperMining,
		"Copper Mine",
		getResourceSprite(GameResource::Copper, false),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::generationBase(g, getBaseCoords(pos), GameResource::Copper);
		},
		resBaseValid(GameResource::Copper)
	},
	{
		{},
		Technology::GoldMining,
		"Gold Mine",
		getResourceSprite(GameResource::Gold, false),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::generationBase(g, getBaseCoords(pos), GameResource::Gold);
		},
		resBaseValid(GameResource::Gold)
	},
	{
		{},
		Technology::Farming,
		"Farm",
		getResourceSprite(GameResource::Wheat, false),
		[&](Game* g, sf::Vector2f pos) {
			std::shared_ptr<Entity> e = EntityPrefabs::generationBase(g, getBaseCoords(pos), GameResource::Wheat);
			setIsFullForBase(g, pos, e);
			return e;
		},
		noResBaseValid
	},
	{
		{},
		Technology::Mills,
		"Mill",
		getResourceSprite(GameResource::Flour, false),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::conversionBase(g, getBaseCoords(pos), GameResource::Flour);
		},
		noResBaseValid
	},
	{
		{},
		Technology::Baking,
		"Bakery",
		getResourceSprite(GameResource::Bread, false),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::conversionBase(g, getBaseCoords(pos), GameResource::Bread);
		},
		noResBaseValid
	},
	{
		{},
		Technology::Orchards,
		"Orchard",
		getResourceSprite(GameResource::Fruit, false),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::generationBase(g, getBaseCoords(pos), GameResource::Fruit);
		},
		noResBaseValid
	},
	{
		{},
		Technology::Breweries,
		"Brewery",
		getResourceSprite(GameResource::Beer, false),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::conversionBase(g, getBaseCoords(pos), GameResource::Beer);
		},
		noResBaseValid
	},
	{
		{},
		Technology::Smelting,
		"Furnace",
		getResourceSprite(GameResource::Steel, false),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::conversionBase(g, getBaseCoords(pos), GameResource::Steel);
		},
		noResBaseValid
	},
	{
		{},
		Technology::Forgery,
		"Forge",
		getResourceSprite(GameResource::Weapons, false),
		[&](Game* g, sf::Vector2f pos) {
			return EntityPrefabs::conversionBase(g, getBaseCoords(pos), GameResource::Weapons);
		},
		noResBaseValid
	},
	{
		{},
		Technology::MilitaryBases,
		"Military Base",
		ResourceManager::get()->getSprite("ships", "cannon.png", false),
		[&](Game* g, sf::Vector2f(pos)) {
			return EntityPrefabs::militaryBase(g, getBaseCoords(pos));
		},
		noResBaseValid
	}
};
#include "Game.h"
