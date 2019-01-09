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
	etype(Bread) \
	etype(Fruit) \
	etype(Beer)

#define etype(x) x,

typedef enum { GAME_RESOURCES } GameResource;

#undef etype
#define etype(x) GameResource::x,

const std::vector<GameResource> ALL_RESOURCES = {
	GAME_RESOURCES
};

#undef etype
#define etype(X) { GameResource::X, #X},

const std::map<GameResource, std::string> RESOURCE_STRINGS = {
	GAME_RESOURCES
};

#undef etype

std::string getResourceString(GameResource res);
#undef etype

// How much the player gets for selling these resources
// Currently set as the length, TBA, change that
#define etype(x) { GameResource::x, (unsigned int)std::string(#x).length() },
const std::map<GameResource, unsigned int> RESOURCE_VALUES = {
	GAME_RESOURCES
};

#undef etype
#undef GAME_RESOURCES

sf::Sprite getResourceSprite(GameResource res, bool center = true);
// Get whether the base generating the given resource needs a resource source
bool generationBaseNeedsSource(GameResource res);