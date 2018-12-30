#pragma once
#include <string>
#include <map>
#include <vector>
#include <SFML/Graphics.hpp>

// Automatically create an enum and a vector of the enum
#define GAME_RESOURCES \
	etype(Stone) \
	etype(Wood) \
	etype(Gold) \
	etype(Plank) \
	etype(Iron) \
	etype(Copper) \
	etype(StoneBrick) \
	etype(StoneStatue) \
	etype(Wheat) \
	etype(Flour) \
	etype(Bread)

#define etype(x) x,

typedef enum { GAME_RESOURCES } GameResource;

#undef etype
#define etype(x) GameResource::x,

const std::vector<GameResource> ALL_RESOURCES = {
	GAME_RESOURCES
};

#undef etype
#define etype(X) { GameResource::X, "X"},

const std::map<GameResource, std::string> RESOURCE_STRINGS = {
	GAME_RESOURCES
};

#undef etype

std::string getResourceString(GameResource res);
#undef etype
#undef GAME_RESOURCE

// How much the player gets for selling these resources
const std::map<GameResource, unsigned int> RESOURCE_VALUES = {
	{GameResource::Stone, 10},
	{GameResource::Wood, 10},
	{GameResource::Gold, 100},
	{GameResource::Plank, 35},
	{GameResource::StoneBrick, 30},
	{GameResource::StoneStatue, 200}
};

sf::Sprite getResourceSprite(GameResource res, bool center = true);
// Get whether the base generating the given resource needs a resource source
bool generationBaseNeedsSource(GameResource res);